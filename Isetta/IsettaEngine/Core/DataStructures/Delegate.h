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
  using KeyPair = std::pair<U16, Action<const ActionArgs&...>>;
  std::list<KeyPair> actions;
  HandleBin handleBin;

 public:
  Delegate() = default;
  Delegate(const Delegate&) = delete;
  Delegate(Delegate&&) = default;
  Delegate& operator=(const Delegate&) = delete;
  Delegate& operator=(Delegate&&) = default;
  ~Delegate() = default;

  U64 Subscribe(Action<const ActionArgs&...> action);
  void Unsubscribe(U64& handle);
  void Invoke(const ActionArgs&... args);
  void Clear();
};

template <typename... ActionArgs>
U64 Delegate<ActionArgs...>::Subscribe(Action<const ActionArgs&...> action) {
  U64 handle = handleBin.GetHandle();
  actions.push_back({handle, action});
  return handle;
}

template <typename... ActionArgs>
void Delegate<ActionArgs...>::Unsubscribe(U64& handle) {
  if (std::find_if(actions.begin(), actions.end(), [handle](const KeyPair& a) {
        return a.first == handle;
      }) != actions.end()) {
    actions.remove_if(
        [handle](const KeyPair& action) { return action.first == handle; });
    handle = 0;
  }
}

template <typename... ActionArgs>
void Delegate<ActionArgs...>::Invoke(const ActionArgs&... args) {
  // In case the callback is unregistering itself
  std::list<Action<const ActionArgs&...>> callbacks;
  for (const KeyPair& callback : actions) {
    callbacks.push_back(callback.second);
  }
  for (const Action<const ActionArgs&...>& callback : callbacks) {
    callback(args...);
  }
}

template <typename ... ActionArgs>
void Delegate<ActionArgs...>::Clear() {
  actions.clear();
  handleBin.Clear();
}
}  // namespace Isetta
