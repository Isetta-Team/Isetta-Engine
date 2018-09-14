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
  FileSystem();

  typedef struct _OVERLAPIOINFO {
    OVERLAPPED overlapped;
    HANDLE hFile;
    char* buf;
    std::function<void(const char*)> callback;
  } OverlapIOInfo;

  void Read(const char* file, OverlapIOInfo* info);
  void Write(const char* file, OverlapIOInfo* info,
             const bool appendData = true);
  void Read(const std::string& file, OverlapIOInfo* info);
  void Write(const std::string& file, OverlapIOInfo* info,
             const bool appendData = true);

  static HANDLE hIOCP;

 private:
  HANDLE CreateNewCompletionPort();
  HANDLE OpenFile(const char* file, const DWORD access, const DWORD share);
  BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                   DWORD completionKey);

  LPCTSTR ErrorMessage(DWORD error);
  void GetError();

  std::unordered_set<HANDLE> handles;
};

}  // namespace Isetta
