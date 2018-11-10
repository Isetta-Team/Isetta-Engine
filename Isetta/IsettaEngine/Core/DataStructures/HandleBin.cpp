/*
 * Copyright (c) 2018 Isetta
 */

#include "HandleBin.h"

#include <limits>

namespace Isetta {

HandleBin::HandleBin() { maxHandle = std::numeric_limits<int>::max(); }
HandleBin::HandleBin(int max) { maxHandle = max; }

int HandleBin::GetHandle() {
  if (handles.size() == 0) {
    if (topHandle > maxHandle) {
      throw std::exception(
          "HandleBin::GetHandle => Trying to get more handles than the max "
          "allowed.");
    }
    return topHandle++;
  } else {
    int handle = *handles.begin();
    handles.erase(handles.begin());
    return handle;
  }
}

void HandleBin::ReturnHandle(int handle) {
  if (handle >= topHandle || handles.find(handle) != handles.end()) {
    throw std::exception(
        "HandleBin::ReturnHandle => Trying to return an unused handle.");
  }

  handles.insert(handle);
}

}  // namespace Isetta
