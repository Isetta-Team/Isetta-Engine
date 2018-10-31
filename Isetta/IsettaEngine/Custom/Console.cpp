/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/Console.h"

#include <stdarg.h>
#include <vector>
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Vector2.h"
#include "Core/Time/Time.h"
#include "Graphics/GUI.h"
#include "Util.h"
#include "imgui/imgui.h"

namespace Isetta {
Console::Console(std::string title, bool isOpen)
    : title{title}, isOpen{isOpen} {
  configCmds = Config::Instance().GetCommands();
  AddCommand("commands", [&](std::string_view) {
    AddLog("CMD\\> Config Commands:\n");
    for (const auto& cmd : configCmds)
      AddLog("- " + std::string{cmd.data()} + "\n");
    AddLog("CMD\\> User Commands:\n");
    for (const auto& cmd : userCmds)
      AddLog("- " + std::string{cmd.first.data()} + "\n");
  });

  verbosityColor[(int)Debug::Verbosity::Info] = Color::white;
  verbosityColor[(int)Debug::Verbosity::Warning] = Color::orange;
  verbosityColor[(int)Debug::Verbosity::Error] = Color::red;
}

int Console::CommandCallbackStub(InputTextCallbackData* data) {
  Console* console = (Console*)data->UserData;
  return console->CommandCallback(data);
}
int Console::CommandCallback(InputTextCallbackData* data) {
  switch ((GUI::InputTextFlags)data->EventFlag) {
    case GUI::InputTextFlags::CallbackCompletion: {
      const char* wordEnd = data->Buf + data->CursorPos;
      const char* wordStart = wordEnd;
      while (wordStart > data->Buf) {
        const char c = wordStart[-1];
        if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
        wordStart--;
      }

      int len = wordEnd - wordStart;
      std::string_view word{wordStart, (size_t)len};
      std::vector<std::string_view> candidates;
      for (int i = 0; i < configCmds.size(); i++) {
        if (configCmds[i].compare(0, len, word) == 0)
          candidates.push_back(configCmds[i]);
      }
      if (candidates.size() == 0) {
        AddLog(Util::StrFormat("CMD\\> No match for \"%.*s\"!\n",
                               (int)(wordEnd - wordStart), wordStart));
      } else if (candidates.size() == 1) {
        data->DeleteChars((int)(wordStart - data->Buf),
                          (int)(wordEnd - wordStart));
        data->InsertChars(data->CursorPos, candidates[0].data());
        data->InsertChars(data->CursorPos, "=");
      } else {
        int matchLen = len;
        for (;;) {
          int c = 0;
          bool allMatch = true;
          c = candidates[0][matchLen];
          for (int i = 1; i < candidates.size() && allMatch; i++) {
            if (c == 0 || c != candidates[i][matchLen]) allMatch = false;
          }
          if (!allMatch) break;
          matchLen++;
        }
        if (matchLen > 0) {
          data->DeleteChars((int)(wordStart - data->Buf),
                            (int)(wordEnd - wordStart));
          data->InsertChars(data->CursorPos, candidates[0].data(),
                            candidates[0].data() + matchLen);
          AddLog("CMD\\> Possible Matches:\n");
          for (const auto& candidate : candidates)
            AddLog("- " + std::string{candidate.data()} + "\n");
        }
      }
      break;
    }
    case GUI::InputTextFlags::CallbackHistory: {
      const int prevHistoryPos = historyPos;
      if (data->EventKey == ImGuiKey_UpArrow) {
        if (historyPos == -1)
          historyPos = history.size() - 1;
        else if (historyPos > 0)
          historyPos--;
      } else if (data->EventKey == ImGuiKey_DownArrow) {
        if (historyPos != -1)
          if (++historyPos >= history.size()) historyPos = -1;
      }

      if (prevHistoryPos != historyPos) {
        std::string_view historyStr =
            (historyPos >= 0) ? history[historyPos] : "";
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, historyStr.data());
      }

      break;
    }
  }
  return 0;
}

// void Console::GetKeyValue(const std::string_view cmd, const int delimPos,
//                          std::string* key, std::string_view* value) {}

void Console::OnEnable() {
  Logger::outputCallback =
      std::bind(&Console::AddLog, this, std::placeholders::_1);
}
void Console::OnDisable() { Logger::outputCallback = NULL; }
void Console::GuiUpdate() {
  GUI::Window(
      rectTransform, title,
      [&]() {
        float width = 60, height = 20, padding = 5;
        RectTransform rect{{padding, 10, width, height}};
        if (GUI::Button(rect, "Clear")) Clear();
        rect.rect.x += width + padding;
        bool copy = GUI::Button(rect, "Copy");
        rect.rect.x += width + padding;

        bool displayVerbosity[(int)Debug::Verbosity::All];
        for (int verbosity = (int)Debug::Verbosity::All - 1;
             verbosity > (int)Debug::Verbosity::Off; verbosity--) {
          displayVerbosity[verbosity] = Logger::verbosityMask.test(verbosity);
          std::string label = Debug::ToString((Debug::Verbosity)verbosity);
          GUI::Toggle(rect, label, &displayVerbosity[verbosity]);
          // TODO(Jacob) change distance
          rect.rect.x += padding + label.length() * 2.3f * padding;
          Logger::verbosityMask.set(verbosity, displayVerbosity[verbosity]);
        }
        displayVerbosity[0] = displayVerbosity[1] == displayVerbosity[2] &&
                              displayVerbosity[2] == displayVerbosity[3];

        // rect.rect.x += padding + width + label.len
        // std::vector<std::string_view> channels;
        // for (int channel = 0; channel <
        // static_cast<int>(Debug::Channel::All);
        //     channel++)
        //  channels.push_back(
        //      Debug::ToString(static_cast<Debug::Channel>(channel)));
        // Logger::channelMask.count
        // GUI::ComboBox<64>(rect, "Channel", &Logger::channelMask, channels);

        rect.anchor = GUI::Pivot::TopRight;
        rect.rect.x = -2.5 * width - 2 * padding;
        GUI::Toggle(rect, "Scroll To Bottom", &scrollToBottom);
        ImGui::Separator();
        rect.anchor = GUI::Pivot::TopLeft;
        rect.rect.SetPosition(padding, rect.rect.y + height + padding);

        const float footerHeightReserve = ImGui::GetStyle().ItemSpacing.y +
                                          ImGui::GetFrameHeightWithSpacing();
        rect.rect.SetSize(0, -footerHeightReserve);
        RectTransform lineRect{{padding, padding, height, width}};
        GUI::Child(
            rect, "scrolling",
            [&]() {
              if (copy) ImGui::LogToClipboard();

              // TextBuffer buf;
              for (const auto& line : log) {
                bool displayed = false;
                for (int verbosity = (int)Debug::Verbosity::Off + 1;
                     verbosity < (int)Debug::Verbosity::All; verbosity++) {
                  if (line.find("[" +
                                Debug::ToString((Debug::Verbosity)verbosity) +
                                "]") != std::string::npos) {
                    displayed = true;
                    if (displayVerbosity[0] || displayVerbosity[verbosity]) {
                      GUI::Text(lineRect, line.data(),
                                GUI::TextStyle{verbosityColor[verbosity]});
                      lineRect.rect.y += height;
                    }
                  }
                }
                if (!displayed) {
                  GUI::Text(lineRect, line.data(), GUI::TextStyle{Color::grey});
                  lineRect.rect.y += height;
                }
              }
              if (scrollToBottom) ImGui::SetScrollHere(1.0f);
            },
            false, GUI::WindowFlags::HorizontalScrollbar);
        ImGui::Separator();
        rect.anchor = GUI::Pivot::BotLeft;
        rect.rect.SetPosition(padding, -5 * padding);
        rect.rect.SetSize(4 * width, height);
        static char command[200];
        if (GUI::InputText(rect, "Command", command, 200, {},
                           GUI::InputTextFlags::EnterReturnsTrue |
                               GUI::InputTextFlags::CallbackCompletion |
                               GUI::InputTextFlags::CallbackHistory,
                           &CommandCallbackStub, (void*)this)) {
          // TODO(Jacob) should this be placed in callback?
          std::string_view cmdStr{command};
          // TODO(Jacob) this signifies a user command?
          int userPos = cmdStr.find("|");
          if (userPos != std::string::npos) {
            std::string_view key = cmdStr.substr(0, userPos);
            std::string_view value =
                cmdStr.substr(userPos + 1, cmdStr.length());
            auto it = userCmds.find(key);
            if (it != userCmds.end()) it->second(value);
          } else {
            int equalPos = cmdStr.find('=');
            if (equalPos != std::string::npos) {
              std::string key = cmdStr.data();
              key = key.substr(0, equalPos);
              std::string_view value =
                  cmdStr.substr(equalPos + 1, cmdStr.length());
              Config::Instance().SetVal(key, value.data());
            }
            lineRect.rect.y += height;
          }
          AddLog("CMD\\> " + std::string{command});
        }
      },
      &isOpen, GUI::WindowStyle{Math::Rect{600, 40, 1000, 900}});
}
void Console::Clear() { log.clear(); }
void Console::AddLog(const std::string_view& format) {
  log.push_back(format.data());
  // scrollToBottom = true;
}
void Console::AddCommand(const std::string_view& cmd,
                         const Action<std::string_view>& callback) {
  userCmds.insert(std::make_pair(cmd, callback));
}
}  // namespace Isetta
