/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/GameMgtLevel/GameMgtLevel.h"
#include "Networking/NetworkManager.h"
#include "Core/Config/Config.h"

using namespace Isetta;


void GameMgtLevel::OnLevelLoad() {
  NetworkManager::Instance().StartHost(CONFIG_VAL(networkConfig.defaultServerIP).c_str());
}
