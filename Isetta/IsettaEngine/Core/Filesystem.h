/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
#include <functional>
#include <string>
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

#define IOCP_EOF 3
#define IOCP_WRITE 1
#define IOCP_CANCEL 0

namespace Isetta {
/**
 * @brief [Singleton] Handles opening/closing, reading, and writing files.
 * Integrated with the Microsoft API
 *
 */
class ISETTA_API Filesystem {
 public:
  /**
   * @brief Singleton class instance
   *
   * @return FileSystem&
   */
  static Filesystem& Instance() {
    static Filesystem instance;
    return instance;
  }

  /**
   * @brief Destroy the File System object, cancel all current processes,
   * close files, I/O completion port, and delete all buffers created
   *
   */
  ~Filesystem();

  /**
   * @brief For ease of use with Microsoft API
   *
   */
  typedef struct _OVERLAPIOINFO {
    /// Microsoft overlapped datastructure used by asynchronus file I/O
    OVERLAPPED overlapped;
    /// Handle of the file associate with the call
    HANDLE hFile;
    /// Buffer which is being read into
    char* buffer;
    /// Callback function for completion of asynchronus operation
    Action<const char*> callback;
  } OverlapIOInfo;

  /**
   * @brief Read the specificed filename (with file path) synchronously
   *
   * @param fileName
   * @return char* the contents of the read file
   */
  char* Read(const char* fileName);
  /**
   * @brief Read the specificed filename (with file path) synchronously
   *
   * @param fileName
   * @return char* the contents of the read file
   */
  char* Read(const std::string& fileName);
  /**
   * @brief Read the specificed filename (with file path) asynchronously
   *
   * @param fileName
   * @param callback called on status completion
   * @return HANDLE a handle to the file which was opened (see Microsoft API)
   */
  HANDLE ReadAsync(const char* fileName,
                   const Action<const char*>& callback = nullptr);
  /**
   * @brief Write to the specificed filename (with file path) asynchronously
   *
   * @param fileName
   * @param contentBuffer is the content to write
   * @param callback called on status completion
   * @param appendData whether the content should be appended to the end of the
   * file, or clobber the file
   * @return HANDLE a handle to the file which was opened (see Microsoft API)
   */
  HANDLE WriteAsync(const char* fileName, const char* contentBuffer,
                    const Action<const char*>& callback = nullptr,
                    const bool appendData = true);
  /**
   * @brief Read the specificed filename (with file path) asynchronously
   *
   * @param fileName
   * @param callback the callback used on status completion
   * @return HANDLE a handle to the file which was opened (see Microsoft API)
   */
  HANDLE ReadAsync(const std::string& fileName,
                   const Action<const char*>& callback = nullptr);
  /**
   * @brief Write to the specificed filename (with file path) asynchronously
   *
   * @param fileName
   * @param contentBuffer is the content to write
   * @param callback called on status completion
   * @param appendData whether the content should be appended to the end of the
   * file, or clobber the file
   * @return HANDLE a handle to the file which was opened (see Microsoft API)
   */
  HANDLE WriteAsync(const std::string& fileName, const char* contentBuffer,
                    const Action<const char*>& callback = nullptr,
                    const bool appendData = true);
  /**
   * @brief Write to the specificed filename (with file path) asynchronously
   *
   * @param fileName
   * @param contentBuffer is the content to write
   * @param callback called on status completion
   * @param appendData whether the content should be appended to the end of the
   * file, or clobber the file
   * @return HANDLE a handle to the file which was opened (see Microsoft API)
   */
  HANDLE WriteAsync(const std::string& fileName,
                    const std::string& contentBuffer,
                    const Action<const char*>& callback = nullptr,
                    const bool appendData = true);
  /**
   * @brief Cancel any/all operations of the given handle
   * (if multiple files have operations to this handle), all will be canceled
   *
   * @param hFile
   * @return true the file was successfully canceled
   * @return false the file was already completed, thus can't be canceled
   */
  // TODO(Jacob) Causes memory leak - add if needed
  // bool Cancel(HANDLE hFile);
  /**
   * @brief Touch a new file with the name and path
   *
   * @param fileName
   */
  void Touch(const char* fileName);
  /**
   * @brief Touch a new file with the name and path
   *
   * @param fileName
   */
  void Touch(const std::string& fileName);
  /**
   * @brief Check if the file exists in the directory (cannot determine if the
   * file doesn't exist or simply the folder structure doesn't)
   *
   * @param file
   * @return BOOL true if the file exists, false otherwise
   */
  BOOL FileExists(const char* file);
  /**
   * @brief GetFileSize returns the size of the file in characters
   *
   * @param fileName
   */
  int GetFileLength(const std::string& fileName);
  /**
   * @brief
   *
   * @param fileName
   */
  static void Concat(const std::initializer_list<std::string>& path,
                     std::string* file);
  /**
   * @brief
   */
  static inline const char PathSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
  }

  /**
   * @brief Get the I/O completion port
   *
   * @return HANDLE to the I/O completion port
   */
  inline HANDLE GethIOCP() { return hIOCP; }

 private:
  /**
   * @brief Construct a new File System object, private b/c Singleton
   *
   */
  Filesystem();

  /**
   * @brief Create a New Completion Port object
   *
   * @return HANDLE to the I/O completion port
   */
  HANDLE CreateNewCompletionPort();
  /**
   * @brief Associate the file with the I/O completion port, calls specified key
   * on completion
   *
   * @param hIoPort
   * @param hFile
   * @param completionKey
   * @return BOOL true if successfully associated
   */
  BOOL AssociateFileCompletionPort(HANDLE hIoPort, HANDLE hFile,
                                   DWORD completionKey);
  /**
   * @brief Create/Open the file (named access because Microsoft API has Create)
   * with the given parameters, see Microsoft documentaion for more details
   *
   * @param fileName including path to the file to open
   * @param access rights (read|write|delete|all)
   * @param share rights whether to allow multiple handles to access the same
   * file
   * @param creation whether the file is opened/created and how its handled on
   * fail, and errors if failed
   * @param async whether the call is asynchronus (overlapped)
   * @return HANDLE the handle to the file opened
   */
  HANDLE AccessFile(const char* fileName, const DWORD access, const DWORD share,
                    const DWORD creation, DWORD async);

  /**
   * @brief Format generic, uncaught Microsoft error messages
   *
   * @param error
   * @return LPCTSTR
   */
  LPCTSTR ErrorMessage(DWORD error);
  /**
   * @brief Get/handle the error associated with accessing a file
   *
   * @return DWORD
   */
  DWORD GetFileError();
  /**
   * @brief Get/handle the error associated with reading/writing to a file
   *
   */
  void GetReadWriteError();

  /// Thread for checking on completion
  HANDLE thread;
  /// Handle for the I/O completion port
  HANDLE hIOCP;
  /// Map to track the handles with their data
  // TODO(Jacob) Causes memory leak - add if needed
  // std::unordered_map<HANDLE, OverlapIOInfo*> overlapInfo;
};

}  // namespace Isetta
