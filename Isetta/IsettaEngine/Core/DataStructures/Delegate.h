/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <algorithm>
#include <list>
#include "Core/IsettaAlias.h"
#include "HandleBin.h"

namespace Isetta {
template <typename... ActionArgs>
class Delegate {
  using KeyPair = std::pair<U16, Action<ActionArgs&&...>>;
  std::list<KeyPair> actions;
  HandleBin handleBin;

 public:
  Delegate() = default;
  Delegate(const Delegate&) = default;
  Delegate(Delegate&&) = default;
  Delegate& operator=(const Delegate&) = default;
  Delegate& operator=(Delegate&&) = default;
  ~Delegate() {
    
  }

  U64 Subscribe(Action<ActionArgs&&...> action);
  void Unsubscribe(U64 handle);
  void Invoke(ActionArgs&&... args);
};

template <typename... ActionArgs>
U64 Delegate<ActionArgs...>::Subscribe(Action<ActionArgs&&...> action) {
  U64 handle = handleBin.GetHandle();
  actions.push_back({handle, action});
  return handle;
}

template <typename... ActionArgs>
void Delegate<ActionArgs...>::Unsubscribe(U64 handle) {
  if (std::find_if(actions.begin(), actions.end(),
                   [handle](const KeyPair& a) {
                     return a.first == handle;
                   }) != actions.end()) {
    actions.remove_if([handle](const KeyPair& action) { return action.first == handle; });
  }
}

template <typename ... ActionArgs>
void Delegate<ActionArgs...>::Invoke(ActionArgs&&... args) {
  // In case the callback is unregistering itself
  std::list<Action<ActionArgs&&...>> callbacks;
  for (const KeyPair& callback : actions) {
    callbacks.push_back(callback.second);
  }
  for (const Action<ActionArgs&&...>& callback : callbacks) {
    callback(args...);
  }
}
}  // namespace Isetta
