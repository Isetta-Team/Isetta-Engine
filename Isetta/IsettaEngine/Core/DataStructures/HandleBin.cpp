/*
 * Copyright (c) 2018 Isetta
 */

#include "HandleBin.h"

#include <limits>

namespace Isetta {

HandleBin::HandleBin()
    : topHandle{1ll}, maxHandle{std::numeric_limits<U64>::max()} {};
HandleBin::HandleBin(U64 start)
    : topHandle{start}, maxHandle{std::numeric_limits<U64>::max()} {};
HandleBin::HandleBin(U64 start, U64 max) : topHandle{start}, maxHandle{max} {};

U64 HandleBin::GetHandle() {
  if (handles.size() == 0) {
    if (topHandle > maxHandle) {
      throw std::exception(
          "HandleBin::GetHandle => Trying to get more handles than the max "
          "allowed.");
    }
    return topHandle++;
  } else {
    int handle = *handles.begin();
    handles.erase(handle);
    return handle;
  }
}

void HandleBin::ReturnHandle(U64 handle) {
  if (handle >= topHandle || handles.find(handle) != handles.end()) {
    throw std::exception(
        "HandleBin::ReturnHandle => Trying to return an unused handle.");
  }

  handles.insert(handle);
}

bool HandleBin::RemoveHandle(U64 handle) {
  if (handle > maxHandle) {
    throw std::exception(
        "HandleBin::RemoveHandle => Trying to remove a greater handle than the "
        "max allowed.");
  }

  if (topHandle > handle && handles.find(handle) == handles.end()) {
    return false;
  } else {
    while (topHandle <= handle) {
      handles.insert(topHandle++);
    }
    handles.erase(handle);
    return true;
  }
}

void HandleBin::Clear() {
  handles.clear();
  topHandle = 1ll;
}
}  // namespace Isetta
