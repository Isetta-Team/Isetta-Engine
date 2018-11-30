/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Color.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Debug.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(Console, Component, false)
private:
const char USER_DELIM = '|', CFG_DELIM = '=';

Array<std::string> log;
Array<std::string> history;
static Array<std::string_view> cfgCmds;
static std::unordered_map<std::string_view,
                          Action<Console* const, std::string_view>>
    userCmds;
int historyPos = -1;
bool scrollToBottom;
static std::list<Console*> consolesOpen;

std::string title;
bool isOpen;
RectTransform rectTransform{{300, 30, 520, 400}};
Color verbosityColor[(int)Debug::Verbosity::All];

static void CommandsCmd(Console* const console, std::string_view params);
static void HistoryCmd(Console* const console, std::string_view params);
static void HelpCmd(Console* const console, std::string_view params);
static int CommandCallbackStub(InputTextCallbackData* data);
int CommandCallback(InputTextCallbackData* data);

public:
Console(std::string title, bool isOpen);
Console(std::string title, bool isOpen, const GUI::WindowStyle& style,
        const Color textColors[(int)Debug::Verbosity::All]);
~Console() override;

void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;

void Clear();
void AddLog(const std::string_view& format);
static void AddCommand(
    const std::string_view& cmd,
    const Action<Console* const, std::string_view>& callback);

inline Array<std::string> GetHistory() { return history; }
void Open();
DEFINE_COMPONENT_END(Console, Component)
}  // namespace Isetta
