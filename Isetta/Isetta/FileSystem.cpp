/*
 * Copyright (c) 2018 Isetta
 */
#include "FileSystem.h"

#include <iostream>

namespace Isetta {
FileSystem::FileSystem() {
  std::cout << "filesys" << std::endl;
  AsyncOStreambuf obuf("async.out");
  std::ostream ostr(&obuf);

  // AsyncOStreambuf ibuf("async.in");
  // std::istream istr(&ibuf);
  std::ifstream in("async.in");

  const int MAX_CHARS = 1023;
  for (std::string line; std::getline(in, line);) {
    // for (char line[MAX_CHARS]; istr.getline(line, MAX_CHARS);) {
    // std::cout << line << std::endl;
    ostr << line << '\n' << std::flush;
  }
}  // namespace Isetta

void AsyncOStreambuf::worker() {
  bool localDone = false;
  std::vector<char> buf;

  while (!localDone) {
    {
      std::unique_lock<std::mutex> guard(this->mutex);
      this->condition.wait(
          guard, [this]() { return !this->queue.empty() || this->isDone; });
      if (!this->queue.empty()) {
        buf.swap(queue.front());
        queue.pop();
      }
      localDone = this->queue.empty() && this->isDone;
    }

    if (!buf.empty()) {
      out->write(buf.data(), std::streamsize(buf.size()));
      buf.clear();
    }
  }
  out->flush();
}

int AsyncOStreambuf::overflow(int c) {
  if (c != std::char_traits<char>::eof()) {
    *this->pptr() = std::char_traits<char>::to_char_type(c);
    this->pbump(1);
  }
  return this->sync() != -1 ? std::char_traits<char>::not_eof(c)
                            : std::char_traits<char>::eof();
}
int AsyncOStreambuf::sync() {
  if (this->pbase() != this->pptr()) {
    this->buffer.resize(std::size_t(this->pptr() - this->pbase()));
    {
      std::unique_lock<std::mutex> guard(this->mutex);
      this->queue.push(std::move(this->buffer));
    }
    this->condition.notify_one();
    this->buffer = std::vector<char>(128);
    this->setp(this->buffer.data(),
               this->buffer.data() + this->buffer.size() - 1);
  }
  return 0;
}
}  // namespace Isetta
