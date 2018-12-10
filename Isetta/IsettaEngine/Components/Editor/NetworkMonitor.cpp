/*
 * Copyright (c) 2018 Isetta
 */
#include "NetworkMonitor.h"

#include "Core/Config/Config.h"
#include "Core/SystemInfo.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Networking/NetworkManager.h"

namespace Isetta {
#ifdef _EDITOR
void NetworkMonitor::GuiUpdate() {
  // Get a string that describes network role
  std::string log = "Offline";

  if (NetworkManager::Instance().IsClient()) {
    log = "Client";
  } else if (NetworkManager::Instance().IsHost()) {
    log = "Host";
  } else if (NetworkManager::Instance().IsServer()) {
    log = "Server";
  }

  static bool isOpen = true;
  // Draw a GUI window showing relevant information
  GUI::Window(
      RectTransform{Math::Rect{300, 80, 200, 300}, GUI::Pivot::Top,
                    GUI::Pivot::Top},
      "Network Monitor",
      [=]() {
        GUI::Text(RectTransform{Math::Rect{5, 0, 200, 50}},
                  Util::StrFormat("Network role: %s", log.c_str()));
        GUI::Text(RectTransform{Math::Rect{5, 20, 200, 50}},
                  Util::StrFormat("MachineName: %s",
                                  SystemInfo::GetMachineName().c_str()));
        GUI::Text(RectTransform{Math::Rect{5, 40, 200, 50}},
                  Util::StrFormat("User Name: %s",
                                  SystemInfo::GetSystemUserName().c_str()));
        GUI::Text(
            RectTransform{Math::Rect{5, 60, 200, 50}},
            Util::StrFormat("IP: %s", SystemInfo::GetIpAddressWithPrefix(
                                          CONFIG_VAL(networkConfig.ipPrefix))
                                          .c_str()));
      },
      &isOpen);
}
#endif
}  // namespace Isetta