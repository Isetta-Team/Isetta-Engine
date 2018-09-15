/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/FileSystem.h"

#include <stdio.h>
#include <tchar.h>
#include <stdexcept>
#include "Core/Debug/Logger.h"

DWORD WINAPI SaveFileWorkerThread(LPVOID empty) {
  ULONG_PTR completionKey;
  BOOL completionStatus;
  DWORD bytesTransferred;
  DWORD err;
  OVERLAPPED* overlap;
  Isetta::FileSystem::OverlapIOInfo* info;

  for (;;) {
    completionStatus = GetQueuedCompletionStatus(
        Isetta::FileSystem::Instance().GethIOCP(), &bytesTransferred,
        &completionKey, &overlap, INFINITE);
    err = GetLastError();

    if (completionStatus) {
      switch (completionKey) {
        case IOCP_WRITE:
          // fprintf(stderr, "wrote %d bytes\n", bytesTransferred);
          break;

        case IOCP_EOF:
          info = (Isetta::FileSystem::OverlapIOInfo*)overlap;
          if (info->callback) {
            info->callback(info->buffer);
          }
          CloseHandle(info->hFile);
          if (info->buffer) {
            delete info->buffer;
          }
          delete info;
          break;
      }
      return 1;
    } else {
      if (overlap != NULL) {
        fprintf(stderr, "overlap not null");
      } else if (err != WAIT_TIMEOUT) {
        fprintf(stderr, "timeout");
        break;
      }
      return 0;
    }
  }
}

namespace Isetta {
FileSystem::FileSystem() {
  hIOCP = CreateNewCompletionPort();
  thread = CreateThread(NULL, 0, SaveFileWorkerThread, NULL, 0, NULL);
}

FileSystem::~FileSystem() {
  if (thread) {
    TerminateThread(thread, THREAD_TERMINATE);
    thread = NULL;
  }
  while (!overlapInfo.empty()) {
    Cancel(overlapInfo.begin()->first);
  }
  if (hIOCP) {
    CloseHandle(hIOCP);
    hIOCP = NULL;
  }
}

HANDLE FileSystem::CreateNewCompletionPort() {
  return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

HANDLE FileSystem::OpenFile(const char* file, const DWORD access,
                            const DWORD share, const DWORD creation) {
  return CreateFile(file, access, share, nullptr, creation,
                    FILE_FLAG_OVERLAPPED, nullptr);
}

BOOL FileSystem::AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                             DWORD completionKey) {
  HANDLE h = CreateIoCompletionPort(hFile, hIoPort, completionKey, 0);
  return h == hIoPort;
}

void FileSystem::GetReadWriteError() {
  DWORD dwError = 0;
  switch (dwError = GetLastError()) {
    case ERROR_IO_PENDING:  // not an error
      break;
    case ERROR_INVALID_USER_BUFFER:
      LOG_ERROR(
          Debug::Channel::FileIO,
          "FileSystem::GetReadWriteError ERROR_INVALID_USER_BUFFER too many "
          "outstanding asynchronous "
          "I/O requests");
      break;
    case ERROR_NOT_ENOUGH_MEMORY:
      LOG_ERROR(
          Debug::Channel::FileIO,
          "FileSystem::GetReadWriteError ERROR_NOT_ENOUGH_MEMORY too many "
          "outstanding asynchronous I/O "
          "requests");
      break;
    case ERROR_NOT_ENOUGH_QUOTA:
      LOG_ERROR(
          Debug::Channel::FileIO,
          "FileSystem::GetReadWriteError ERROR_NOT_ENOUGH_QUOTA process's "
          "buffer could not be page-locked");
      break;
    case ERROR_INSUFFICIENT_BUFFER:
      LOG_ERROR(
          Debug::Channel::FileIO,
          "FileSystem::GetReadWriteError ERROR_INSUFFICIENT_BUFFER read from "
          "a mailslot that has a buffer that is too small");
      break;
    case ERROR_OPERATION_ABORTED:
      LOG_WARNING(Debug::Channel::FileIO,
                  "FileSystem::GetError ERROR_OPERATION_ABORTED");
    default: {
      // Decode any other errors codes.
      LPCTSTR errMsg = ErrorMessage(dwError);
      _tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
      LocalFree((LPVOID)errMsg);
    }
  }
}

DWORD FileSystem::GetFileError() {
  DWORD dwError = 0;
  switch (dwError = GetLastError()) {
    case 0:  // not an error
      break;
    case ERROR_ALREADY_EXISTS:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetFileError ERROR_ALREADY_EXISTS file already "
                "exists, cannot be created");
      break;
    case ERROR_FILE_EXISTS:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetFileError ERROR_FILE_EXISTS file cannot be "
                "created, file exists");
      break;
    case ERROR_FILE_NOT_FOUND:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetFileError ERROR_FILE_NOT_FOUND file cannot be "
                "opened, not found");
      break;
    default: {
      // Decode any other errors codes.
      LPCTSTR errMsg = ErrorMessage(dwError);
      _tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
      LocalFree((LPVOID)errMsg);
    }
  }
  return dwError;
}

LPCTSTR FileSystem::ErrorMessage(DWORD error) {
  LPVOID lpMsgBuf;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

  return ((LPCTSTR)lpMsgBuf);
}

HANDLE FileSystem::Read(const char* fileName,
                        const std::function<void(const char*)> callback) {
  HANDLE hFile = OpenFile(fileName, GENERIC_READ, NULL, OPEN_EXISTING);
  if (GetFileError()) {
    return NULL;
  }
  AssociateFileCompletionPort(hIOCP, hFile, 1);

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  DWORD dwBytesRead = 0;

  OverlapIOInfo* info = new OverlapIOInfo{};
  info->hFile = hFile;
  info->callback = callback;
  info->buffer = new char[dwFileSize + 1];
  info->buffer[dwFileSize] = '\0';
  overlapInfo.insert({hFile, info});

  if (!ReadFile(hFile, info->buffer, dwFileSize, &dwBytesRead,
                &info->overlapped)) {
    GetReadWriteError();
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));

  return hFile;
}

HANDLE FileSystem::Write(const char* fileName, const char* contentBuffer,
                         const std::function<void(const char*)> callback,
                         const bool appendData) {
  HANDLE hFile;
  if (appendData) {
    hFile = OpenFile(fileName, GENERIC_WRITE, NULL, OPEN_EXISTING);
  } else {
    hFile = OpenFile(fileName, GENERIC_WRITE, NULL, TRUNCATE_EXISTING);
    if (GetLastError() == ERROR_FILE_NOT_FOUND) {
      hFile = OpenFile(fileName, GENERIC_WRITE, NULL, CREATE_NEW);
    }
  }
  if (GetFileError()) {
    return NULL;
  }

  AssociateFileCompletionPort(hIOCP, hFile, IOCP_WRITE);

  DWORD dwBufferSize = strlen(contentBuffer);
  DWORD dwBytesRead = 0;

  OverlapIOInfo* info = new OverlapIOInfo{};
  info->hFile = hFile;
  info->callback = callback;
  info->buffer = new char[dwBufferSize + 1];
  info->buffer[dwBufferSize] = '\0';
  strncpy_s(info->buffer, dwBufferSize + 1, contentBuffer, dwBufferSize);
  overlapInfo.insert({hFile, info});

  if (appendData) {
    info->overlapped.Offset += GetFileSize(hFile, NULL);
  }
  if (!WriteFile(info->hFile, info->buffer, strlen(info->buffer), &dwBytesRead,
                 &info->overlapped)) {
    GetReadWriteError();
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));
  return hFile;
}

HANDLE FileSystem::Read(const std::string& fileName,
                        const std::function<void(const char*)> callback) {
  return Read(fileName.c_str(), callback);
}
HANDLE FileSystem::Write(const std::string& fileName, const char* contentBuffer,
                         const std::function<void(const char*)> callback,
                         const bool appendData) {
  return Write(fileName.c_str(), contentBuffer, callback, appendData);
}
HANDLE FileSystem::Write(const std::string& fileName,
                         const std::string& contentBuffer,
                         const std::function<void(const char*)> callback,
                         const bool appendData) {
  return Write(fileName.c_str(), contentBuffer.c_str(), callback, appendData);
}

bool FileSystem::Cancel(HANDLE hFile) {
  OverlapIOInfo* info = overlapInfo[hFile];
  bool completionStatus = info;
  if (completionStatus) {
    CancelIo(info->hFile);
    CloseHandle(info->hFile);
    if (info->buffer) {
      delete info->buffer;
    }
    delete info;
  }
  overlapInfo.erase(hFile);
  return completionStatus;
}

}  // namespace Isetta
