/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Filesystem.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <stdexcept>

#include "Core/Debug/Logger.h"

DWORD WINAPI SaveFileWorkerThread(LPVOID empty) {
  ULONG_PTR completionKey;
  BOOL completionStatus;
  DWORD bytesTransferred;
  DWORD err;
  OVERLAPPED* overlap;
  Isetta::Filesystem::OverlapIOInfo* info;

  for (;;) {
    completionStatus = GetQueuedCompletionStatus(
        Isetta::Filesystem::Instance().GethIOCP(), &bytesTransferred,
        &completionKey, &overlap, INFINITE);
    err = GetLastError();

    if (completionStatus) {
      switch (completionKey) {
        case IOCP_WRITE:
          // fprintf(stderr, "wrote %d bytes\n", bytesTransferred);
          break;

        case IOCP_EOF:
          // printf("eof\n");
          info = (Isetta::Filesystem::OverlapIOInfo*)overlap;
          if (info->callback) {
            info->callback(info->buffer);
          }
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
Filesystem::Filesystem() {
  hIOCP = CreateNewCompletionPort();
  thread = CreateThread(NULL, 0, SaveFileWorkerThread, NULL, 0, NULL);
}

Filesystem::~Filesystem() {
  if (thread) {
    TerminateThread(thread, THREAD_TERMINATE);
    thread = NULL;
  }
  // while (!overlapInfo.empty()) {
  //   Cancel(overlapInfo.begin()->first);
  // }
  if (hIOCP) {
    CloseHandle(hIOCP);
    hIOCP = NULL;
  }
}

HANDLE Filesystem::CreateNewCompletionPort() {
  return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

HANDLE Filesystem::AccessFile(const char* filePath, const DWORD access,
                              const DWORD share, const DWORD creation,
                              DWORD async) {
  if (!FileExists(filePath)) {
    const char* delim = "\\";
    char* nextToken;
    char* filePathTok = new char[strlen(filePath) + 1];
    strncpy_s(filePathTok, strlen(filePath) + 1, filePath,
              strlen(filePath) + 1);
    char* folder = strtok_s(filePathTok, delim, &nextToken);
    while (folder != NULL) {
      char* next = strtok_s(NULL, delim, &nextToken);
      if (next == NULL) {
        break;
      }
      CreateDirectory(folder, NULL);
      folder = next;
    }
    delete filePathTok;
  }
  return CreateFile(filePath, access, share, NULL, creation, async, NULL);
}

BOOL Filesystem::AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                             DWORD completionKey) {
  HANDLE h = CreateIoCompletionPort(hFile, hIoPort, completionKey, 0);
  return h == hIoPort;
}

BOOL Filesystem::FileExists(const char* file) {
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = FindFirstFile(file, &findFileData);
  bool exists = hFind != INVALID_HANDLE_VALUE;
  if (exists) {
    FindClose(hFind);
  }
  return exists;
}

void Filesystem::GetReadWriteError() {
  DWORD dwError = 0;
  switch (dwError = GetLastError()) {
    case ERROR_IO_PENDING:  // not an error
      break;
    case ERROR_INVALID_USER_BUFFER:
      throw std::exception(
          "Filesystem::GetReadWriteError() => ERROR_INVALID_USER_BUFFER too "
          "many outstanding asynchronous I/O requests");
      break;
    case ERROR_NOT_ENOUGH_MEMORY:
      throw std::exception(
          "Filesystem::GetReadWriteError() => ERROR_NOT_ENOUGH_MEMORY too many "
          "outstanding asynchronous I/O "
          "requests");
      break;
    case ERROR_NOT_ENOUGH_QUOTA:
      throw std::exception(
          "Filesystem::GetReadWriteError() => ERROR_NOT_ENOUGH_QUOTA process's "
          "buffer could not be page-locked");
      break;
    case ERROR_INSUFFICIENT_BUFFER:
      throw std::exception(
          "Filesystem::GetReadWriteError() => ERROR_INSUFFICIENT_BUFFER read "
          "from a mailslot that has a buffer that is too small");
      break;
    case ERROR_OPERATION_ABORTED:
      throw std::exception("Filesystem::GetError() => ERROR_OPERATION_ABORTED");
    default: {
      // Decode any other errors codes.
      LPCTSTR errMsg = ErrorMessage(dwError);
      _tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
      LocalFree((LPVOID)errMsg);
      char buffer[1023];
      int n = sprintf_s(buffer, 1023, "GetOverlappedResult failed (%d): %s\n",
                        dwError, errMsg);
      buffer[n] = '\0';
      throw std::exception(buffer);
    }
  }
}

DWORD Filesystem::GetFileError() {
  DWORD dwError = 0;
  switch (dwError = GetLastError()) {
    case 0:  // not an error
      break;
    case ERROR_ALREADY_EXISTS:  // not necessarily an error
      // throw std::exception(
      //    "Filesystem::GetFileError ERROR_ALREADY_EXISTS file already "
      //    "exists, cannot be created");
      break;
    case ERROR_FILE_EXISTS:
      throw std::exception(
          "Filesystem::GetFileError ERROR_FILE_EXISTS file cannot be "
          "created, file exists");
      break;
    case ERROR_FILE_NOT_FOUND:
      throw std::exception(
          "Filesystem::GetFileError ERROR_FILE_NOT_FOUND file cannot be "
          "opened, not found");
      break;
    default: {
      // Decode any other errors codes.
      LPCTSTR errMsg = ErrorMessage(dwError);
      _tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"), dwError, errMsg);
      LocalFree((LPVOID)errMsg);
      char buffer[1023];
      int n = sprintf_s(buffer, 1023, "GetOverlappedResult failed (%d): %s\n",
                        dwError, errMsg);
      buffer[n] = '\0';
      throw std::exception(buffer);
    }
  }
  return dwError;
}

LPCTSTR Filesystem::ErrorMessage(DWORD error) {
  LPVOID lpMsgBuf;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

  return ((LPCTSTR)lpMsgBuf);
}

char* Filesystem::Read(const char* filename) {
  HANDLE hFile = AccessFile(filename, GENERIC_READ, NULL, OPEN_EXISTING, NULL);
  try {
    GetFileError();
  } catch (const std::exception& e) {
    LOG_ERROR(
        Debug::Channel::FileIO,
        "Filesystem::Read => file: " + std::string{filename} + "\n" + e.what());
    throw e;
    return NULL;
  }

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  DWORD dwBytesRead = 0;

  char* buffer = new char[dwFileSize + 1];

  if (!ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL)) {
    try {
      GetReadWriteError();
    } catch (const std::exception& e) {
      LOG_ERROR(Debug::Channel::FileIO,
                "Filesystem::Read => file: " + std::string{filename} + "\n" +
                    e.what());
      throw e;
    }
  }
  CloseHandle(hFile);
  buffer[dwFileSize] = '\0';
  return buffer;
}

HANDLE Filesystem::ReadAsync(const char* filename,
                             const Action<const char*>& callback) {
  HANDLE hFile = AccessFile(filename, GENERIC_READ, NULL, OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED);
  try {
    GetFileError();
  } catch (const std::exception& e) {
    LOG_ERROR(Debug::Channel::FileIO,
              "Filesystem::ReadAsync => file: " + std::string{filename} + "\n" +
                  e.what());
    throw e;
    return NULL;
  }
  AssociateFileCompletionPort(hIOCP, hFile, IOCP_WRITE);

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  DWORD dwBytesRead = 0;

  OverlapIOInfo* info = new OverlapIOInfo{};
  info->hFile = hFile;
  info->callback = callback;
  info->buffer = new char[dwFileSize + 1];
  info->buffer[dwFileSize] = '\0';
  // overlapInfo.insert({hFile, info});

  if (!ReadFile(hFile, info->buffer, dwFileSize, &dwBytesRead,
                &info->overlapped)) {
    try {
      GetReadWriteError();
    } catch (const std::exception& e) {
      LOG_ERROR(Debug::Channel::FileIO,
                "Filesystem::Read => file: " + std::string{filename} + "\n" +
                    e.what());
      throw e;
    }
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));

  return hFile;
}

HANDLE Filesystem::WriteAsync(const char* filename, const char* contentBuffer,
                              const Action<const char*>& callback,
                              const bool appendData) {
  HANDLE hFile;
  if (appendData) {
    hFile = AccessFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING,
                       FILE_FLAG_OVERLAPPED);
  } else {
    hFile = AccessFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS,
                       FILE_FLAG_OVERLAPPED);
  }
  try {
    GetFileError();
  } catch (const std::exception& e) {
    LOG_ERROR(Debug::Channel::FileIO,
              "Filesystem::ReadAsync => file: " + std::string{filename} + "\n" +
                  e.what());
    throw e;
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
  // overlapInfo.insert({hFile, info});

  if (appendData) {
    info->overlapped.Offset += GetFileSize(hFile, NULL);
  }
  if (!WriteFile(info->hFile, info->buffer, strlen(info->buffer), &dwBytesRead,
                 &info->overlapped)) {
    try {
      GetReadWriteError();
    } catch (const std::exception& e) {
      LOG_ERROR(Debug::Channel::FileIO,
                "Filesystem::Read => file: " + std::string{filename} + "\n" +
                    e.what());
      throw e;
    }
  }
  PostQueuedCompletionStatus(hIOCP, 0, IOCP_EOF, &(info->overlapped));
  return hFile;
}

char* Filesystem::Read(const std::string& fileName) {
  return Read(fileName.c_str());
}
HANDLE Filesystem::ReadAsync(const std::string& fileName,
                             const Action<const char*>& callback) {
  return ReadAsync(fileName.c_str(), callback);
}
HANDLE Filesystem::WriteAsync(const std::string& fileName,
                              const char* contentBuffer,
                              const Action<const char*>& callback,
                              const bool appendData) {
  return WriteAsync(fileName.c_str(), contentBuffer, callback, appendData);
}
HANDLE Filesystem::WriteAsync(const std::string& fileName,
                              const std::string& contentBuffer,
                              const Action<const char*>& callback,
                              const bool appendData) {
  return WriteAsync(fileName.c_str(), contentBuffer.c_str(), callback,
                    appendData);
}

// bool Filesystem::Cancel(HANDLE hFile) {
//  OverlapIOInfo* info = overlapInfo[hFile];
//  bool completionStatus = info == nullptr;
//  if (!completionStatus) {
//    CancelIo(info->hFile);
//    CloseHandle(info->hFile);
//    if (info->buffer) {
//      delete info->buffer;
//    }
//    delete info;
//    overlapInfo[hFile] == nullptr;
//  }
//  overlapInfo.erase(hFile);
//  return completionStatus;
//}

void Filesystem::Touch(const char* filename) {
  HANDLE hFile = AccessFile(filename, NULL, NULL, CREATE_ALWAYS, NULL);
  try {
    GetFileError();
  } catch (const std::exception& e) {
    LOG_ERROR(Debug::Channel::FileIO,
              "Filesystem::ReadAsync => file: " + std::string{filename} + "\n" +
                  e.what());
    throw e;
  }
  CloseHandle(hFile);
}
void Filesystem::Touch(const std::string& filename) { Touch(filename.c_str()); }

int Filesystem::GetFileLength(const std::string& filename) {
  HANDLE hFile = AccessFile(filename.c_str(), FILE_READ_DATA, FILE_SHARE_READ,
                            OPEN_EXISTING, NULL);
  try {
    GetFileError();
  } catch (const std::exception& e) {
    LOG_ERROR(Debug::Channel::FileIO,
              "Filesystem::ReadAsync => file: " + std::string{filename} + "\n" +
                  e.what());
    throw e;
    return NULL;
  }

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  CloseHandle(hFile);
  return dwFileSize;
}

void Filesystem::Concat(const std::initializer_list<std::string>& path,
                        std::string* file) {
  std::string folder = "";
  for (auto p : path) {
    folder += p + PathSeparator();
  }
  *file = folder + *file;
}

}  // namespace Isetta
