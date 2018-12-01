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
/**
 * @brief Console has all log messages, an command input, and ability to select
 * channel and verbosity
 *
 */
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
/**
 * @brief Construct a new Console object
 *
 * @param title of window
 * @param isOpen whether window starts open
 */
Console(std::string title, bool isOpen);
/**
 * @brief Construct a new Console object
 *
 * @param title of window
 * @param isOpen whether window starts open
 * @param style window style
 * @param textColors colors of text for info, warning, error, disabled
 */
Console(std::string title, bool isOpen, const GUI::WindowStyle& style,
        const Color textColors[(int)Debug::Verbosity::All]);
~Console() override;

void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;

/**
 * @brief Clears all output logs
 *
 */
void Clear();
/**
 * @brief Add a log to the console
 *
 * @param format string to be added
 * [Warning] if contains [Info]/[Warning]/[Error] will be hightlighted
 * corresponding color
 */
void AddLog(const std::string_view& format);
/**
 * @brief Add command to the console, commands denoted "cmd|args"
 *
 * @param cmd string to type in the console window to do callback (followed by
 * |)
 * @param callback to run when command is called
 */
static void AddCommand(
    const std::string_view& cmd,
    const Action<Console* const, std::string_view>& callback);

/**
 * @brief Get the history of commands
 *
 * @return Array<std::string> returns all the commands
 */
inline Array<std::string> GetHistory() { return history; }
/**
 * @brief Opens the window
 *
 */
void Open();
DEFINE_COMPONENT_END(Console, Component)
}  // namespace Isetta
