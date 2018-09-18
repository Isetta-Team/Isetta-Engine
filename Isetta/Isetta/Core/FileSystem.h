/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
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

  char* Read(const char* fileName);
  char* Read(const std::string& fileName);
  HANDLE ReadAsync(const char* fileName,
                   const std::function<void(const char*)>& callback = nullptr);
  HANDLE WriteAsync(const char* fileName, const char* contentBuffer,
                    const std::function<void(const char*)>& callback = nullptr,
                    const bool appendData = true);
  HANDLE ReadAsync(const std::string& fileName,
                   const std::function<void(const char*)>& callback = nullptr);
  HANDLE WriteAsync(const std::string& fileName, const char* contentBuffer,
                    const std::function<void(const char*)>& callback = nullptr,
                    const bool appendData = true);
  HANDLE WriteAsync(const std::string& fileName,
                    const std::string& contentBuffer,
                    const std::function<void(const char*)>& callback = nullptr,
                    const bool appendData = true);
  bool Cancel(HANDLE hFile);
  HANDLE AccessFile(const char* file, const DWORD access, const DWORD share,
                    const DWORD creation, DWORD async);

  inline HANDLE GethIOCP() { return hIOCP; }

 private:
  FileSystem();

  HANDLE CreateNewCompletionPort();
  BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                   DWORD completionKey);
  BOOL FileExists(const char* file);

  LPCTSTR ErrorMessage(DWORD error);
  DWORD GetFileError();
  void GetReadWriteError();

  HANDLE thread;
  HANDLE hIOCP;
  std::unordered_map<HANDLE, OverlapIOInfo*> overlapInfo;
};

}  // namespace Isetta
