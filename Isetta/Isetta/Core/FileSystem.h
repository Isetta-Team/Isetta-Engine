/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <windows.h>
#include <functional>
#include <string>
#include <unordered_set>

#define IOCP_NOMORE 3
#define IOCP_WRITE 1

namespace Isetta {
class FileSystem {
 public:
  static FileSystem& Instance() {
    static FileSystem instance;
    return instance;
  }

  typedef struct _OVERLAPIOINFO {
    OVERLAPPED overlapped;
    HANDLE hFile;
    char* buffer;
    std::function<void(const char*)> callback;
  } OverlapIOInfo;

  void Read(const char* fileName,
            const std::function<void(const char*)> callback = nullptr);
  void Write(const char* fileName, const char* contentBuffer,
             const std::function<void(const char*)> callback = nullptr,
             const bool appendData = true);
  void Read(const std::string& fileName,
            const std::function<void(const char*)> callback = nullptr);
  void Write(const std::string& fileName, const char* contentBuffer,
             std::function<void(const char*)> callback = nullptr,
             const bool appendData = true);
  void Write(const std::string& fileName, const std::string& contentBuffer,
             std::function<void(const char*)> callback = nullptr,
             const bool appendData = true);

  static HANDLE hIOCP;

 private:
  FileSystem();

  HANDLE CreateNewCompletionPort();
  HANDLE OpenFile(const char* file, const DWORD access, const DWORD share);
  BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                   DWORD completionKey);

  LPCTSTR ErrorMessage(DWORD error);
  void GetError();
};

}  // namespace Isetta
