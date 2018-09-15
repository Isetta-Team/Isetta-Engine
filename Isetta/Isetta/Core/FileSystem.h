/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <windows.h>
#include <functional>
#include <string>
#include <unordered_map>

#define IOCP_EOF 3
#define IOCP_WRITE 1
#define IOCP_CANCEL 0

namespace Isetta {
class FileSystem {
 public:
  static FileSystem& Instance() {
    static FileSystem instance;
    return instance;
  }
  ~FileSystem();

  typedef struct _OVERLAPIOINFO {
    OVERLAPPED overlapped;
    HANDLE hFile;
    char* buffer;
    std::function<void(const char*)> callback;
  } OverlapIOInfo;

  HANDLE Read(const char* fileName,
              const std::function<void(const char*)>& callback = nullptr);
  HANDLE Write(const char* fileName, const char* contentBuffer,
               const std::function<void(const char*)>& callback = nullptr,
               const bool appendData = true);
  HANDLE Read(const std::string& fileName,
              const std::function<void(const char*)>& callback = nullptr);
  HANDLE Write(const std::string& fileName, const char* contentBuffer,
               const std::function<void(const char*)>& callback = nullptr,
               const bool appendData = true);
  HANDLE Write(const std::string& fileName, const std::string& contentBuffer,
               const std::function<void(const char*)>& callback = nullptr,
               const bool appendData = true);
  bool Cancel(HANDLE hFile);

  inline HANDLE GethIOCP() { return hIOCP; }

 private:
  FileSystem();

  HANDLE CreateNewCompletionPort();
  HANDLE OpenFile(const char* file, const DWORD access, const DWORD share,
                  const DWORD creation);
  BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                   DWORD completionKey);

  LPCTSTR ErrorMessage(DWORD error);
  DWORD GetFileError();
  void GetReadWriteError();

  HANDLE thread;
  HANDLE hIOCP;
  std::unordered_map<HANDLE, OverlapIOInfo*> overlapInfo;
};

}  // namespace Isetta
