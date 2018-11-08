/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include <unordered_set>
#include "Core/Color.h"
#include "Core/DataStructures/Vector.h"
#include "Core/Debug/Debug.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(Console, Component, false)
private:
Vector<std::string> log;
Vector<std::string> history;
static Vector<std::string_view> cfgCmds;
static std::unordered_map<std::string_view,
                          Action<Console* const, std::string_view>>
    userCmds;
int historyPos = -1;
bool scrollToBottom;
static std::list<Console*> consolesOpen;

std::string title;
bool isOpen;
RectTransform rectTransform{{100, 10, 520, 400}};

// Color defaultTextColor = Color::grey;
Color verbosityColor[(int)Debug::Verbosity::All];

static int CommandCallbackStub(InputTextCallbackData* data);
int CommandCallback(InputTextCallbackData* data);

const char USER_DELIM = '|', CFG_DELIM = '=';

public:
Console(std::string title, bool isOpen);
~Console() override;
// Console(std::string title, bool isOpen, const Color& window);

void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;

void Clear();
void AddLog(const std::string_view& format);
// TODO(Jacob) should this be public?
static void AddCommand(
    const std::string_view& cmd,
    const Action<Console* const, std::string_view>& callback);

inline Vector<std::string> GetHistory() { return history; }
inline void Open() { isOpen = true; }
END_COMPONENT(Console, Component)
}  // namespace Isetta
