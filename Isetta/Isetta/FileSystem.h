/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "iostream"
#include "string"

namespace Isetta {
class FileSystem {
 public:
  FileSystem();
};

class AsyncOStreambuf : public std::streambuf {
 public:
  AsyncOStreambuf(const std::string& name)
      : buffer(128), thread(&AsyncOStreambuf::worker, this) {
    std::allocator<std::ofstream> alloc;
    alloc.allocate(sizeof(std::ofstream));
    out = std::allocate_shared<std::ofstream>(alloc, name);
    // out = new std::ofstream(name);
    this->setp(this->buffer.data(),
               this->buffer.data() + this->buffer.size() - 1);
  }
  ~AsyncOStreambuf() {
    // delete (out);
    std::unique_lock<std::mutex>(this->mutex), (this->isDone = true);
    condition.notify_one();
    thread.join();
  }

  std::shared_ptr<std::ofstream> out;
  std::mutex mutex;
  std::condition_variable condition;
  std::queue<std::vector<char>> queue;
  std::vector<char> buffer;
  bool isDone = false;
  std::thread thread;

  int overflow(int c) override;
  int sync() override;
  void worker();
};

}  // namespace Isetta
