/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
BEGIN_COMPONENT(Inspector, Component, true)
private:
std::string title;
bool isOpen = true;
RectTransform rectTrans{{0, 0, 350, 300}, GUI::Pivot::TopRight};

public:
class Transform* transform;

Inspector(std::string title, bool isOpen);
void GuiUpdate() override;
END_COMPONENT(Inspector, Component)
}  // namespace Isetta

