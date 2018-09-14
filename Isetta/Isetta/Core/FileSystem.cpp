/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/FileSystem.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "Core/Debug/Assert.h"
#include "Core/Debug/Logger.h"

DWORD WINAPI SaveFileWorkerThread(LPVOID empty) {
  ULONG_PTR completionKey;
  BOOL completionStatus;
  DWORD bytesTransferred;
  DWORD err;
  OVERLAPPED* overlap;
  Isetta::FileSystem::OverlapIOInfo* info;

  for (;;) {
    completionStatus =
        GetQueuedCompletionStatus(Isetta::FileSystem::hIOCP, &bytesTransferred,
                                  &completionKey, &overlap, INFINITE);
    err = GetLastError();

    if (completionStatus) {
      switch (completionKey) {
        case IOCP_WRITE:
          // fprintf(stderr, "wrote %d bytes\n", bytesTransferred);
          break;

        case IOCP_NOMORE:
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
    } else {
      if (overlap != NULL) {
        fprintf(stderr, "overlap not null");
      } else if (err != WAIT_TIMEOUT) {
        fprintf(stderr, "timeout");
      }
    }
  }
}

namespace Isetta {
HANDLE FileSystem::hIOCP;
FileSystem::FileSystem() {
  hIOCP = CreateNewCompletionPort();
  CreateThread(NULL, 0, SaveFileWorkerThread, NULL, 0, NULL);
}

HANDLE FileSystem::CreateNewCompletionPort() {
  return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

HANDLE FileSystem::OpenFile(const char* file, const DWORD access,
                            const DWORD share) {
  return CreateFile(file, access, share, nullptr, OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED, nullptr);
}

BOOL FileSystem::AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                             DWORD completionKey) {
  HANDLE h = CreateIoCompletionPort(hFile, hIoPort, completionKey, 0);
  return h == hIoPort;
}

void FileSystem::GetError() {
  DWORD dwError = 0;
  switch (dwError = GetLastError()) {
    case ERROR_IO_PENDING:  // not an error
      break;
    case ERROR_INVALID_USER_BUFFER:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetError ERROR_INVALID_USER_BUFFER too many "
                "outstanding asynchronous "
                "I/O requests");
      break;
    case ERROR_NOT_ENOUGH_MEMORY:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetError ERROR_NOT_ENOUGH_MEMORY too many "
                "outstanding asynchronous I/O "
                "requests");
      break;
    case ERROR_NOT_ENOUGH_QUOTA:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetError ERROR_NOT_ENOUGH_QUOTA process's "
                "buffer could not be page-locked");
      break;
    case ERROR_INSUFFICIENT_BUFFER:
      LOG_ERROR(Debug::Channel::FileIO,
                "FileSystem::GetError ERROR_INSUFFICIENT_BUFFER read from "
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

LPCTSTR FileSystem::ErrorMessage(DWORD error) {
  LPVOID lpMsgBuf;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

  return ((LPCTSTR)lpMsgBuf);
}

void FileSystem::Read(const char* fileName,
                      const std::function<void(const char*)> callback) {
  HANDLE hFile = OpenFile(fileName, GENERIC_READ, FILE_SHARE_READ);
  AssociateFileCompletionPort(hIOCP, hFile, 1);
  DWORD dwFileSize = GetFileSize(hFile, NULL);
  DWORD dwBytesRead = 0;
  OverlapIOInfo* info = new OverlapIOInfo{};
  info->hFile = hFile;
  info->callback = callback;
  info->buffer = new char[dwFileSize + 1];
  info->buffer[dwFileSize] = '\0';
  if (!ReadFile(hFile, info->buffer, dwFileSize, &dwBytesRead,
                &info->overlapped)) {
    GetError();
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_NOMORE, &(info->overlapped));
}

void FileSystem::Write(const char* fileName, const char* contentBuffer,
                       const std::function<void(const char*)> callback,
                       const bool appendData) {
  HANDLE hFile = OpenFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE);
  AssociateFileCompletionPort(hIOCP, hFile, IOCP_WRITE);
  DWORD dwFileSize = strlen(contentBuffer);
  DWORD dwBytesRead = 0;
  OverlapIOInfo* info = new OverlapIOInfo{};
  info->hFile = hFile;
  info->callback = callback;
  info->buffer = new char[dwFileSize + 1];
  info->buffer[dwFileSize] = '\0';
  strncpy_s(info->buffer, dwFileSize + 1, contentBuffer, dwFileSize);
  if (appendData) {
    info->overlapped.Offset += GetFileSize(hFile, NULL);
  }
  if (!WriteFile(info->hFile, info->buffer, strlen(info->buffer), &dwBytesRead,
                 &info->overlapped)) {
    GetError();
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_NOMORE, &(info->overlapped));
}

void FileSystem::Read(const std::string& fileName,
                      const std::function<void(const char*)> callback) {
  Read(fileName.c_str(), callback);
}
void FileSystem::Write(const std::string& fileName, const char* contentBuffer,
                       const std::function<void(const char*)> callback,
                       const bool appendData) {
  Write(fileName.c_str(), contentBuffer, callback, appendData);
}

void FileSystem::Write(const std::string& fileName,
                       const std::string& contentBuffer,
                       const std::function<void(const char*)> callback,
                       const bool appendData) {
  Write(fileName.c_str(), contentBuffer.c_str(), callback, appendData);
}

}  // namespace Isetta
