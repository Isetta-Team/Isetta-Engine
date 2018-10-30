/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_set>
#include <vector>
#include "Core/Color.h"
#include "Core/Debug/Debug.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
class Console : public Component {
 private:
  std::vector<std::string> log;
  std::vector<std::string_view> history;
  std::vector<std::string_view> configCmds;
  std::unordered_map<std::string_view, Action<std::string_view>> userCmds;
  int historyPos;
  bool scrollToBottom;

  std::string title;
  bool isOpen;
  RectTransform rectTransform{{100, 10, 520, 400}};

  Color verbosityColor[(int)Debug::Verbosity::All];

  static int CommandCallbackStub(InputTextCallbackData* data);
  int CommandCallback(InputTextCallbackData* data);

  // TODO(Jacob) not used but could be
  // void GetKeyValue(const std::string_view cmd, const int delimPos,
  //                 std::string* key, std::string_view* value);

 public:
  Console(std::string title, bool isOpen);

  void OnEnable() override;
  void OnDisable() override;
  void GuiUpdate() override;

  void Clear();
  void AddLog(const std::string_view& format);
  // TODO(Jacob) should this be public?
  void AddCommand(const std::string_view& cmd,
                  const Action<std::string_view>& callback);
};
}  // namespace Isetta
