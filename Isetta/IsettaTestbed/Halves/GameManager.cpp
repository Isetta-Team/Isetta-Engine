/*
 * Copyright (c) 2018 Isetta
 */
#include "GameManager.h"

#include "Bullet.h"
#include "Core/Math/Random.h"
#include "Custom/IsettaCore.h"
#include "Graphics/Font.h"
#include "Graphics/GUI.h"
#include "PlayerController.h"
#include "Zombie.h"

namespace Isetta {
std::vector<Entity*> GameManager::zombies;
int GameManager::score = 0;

void GameManager::Start() {
  font = Font::AddFontFromFile("Resources/Fonts/ZOMBIE.TTF", 48.0f);
}

void GameManager::OnEnable() {
  zombies.reserve(poolSize);
  for (int i = 0; i < poolSize; i++) {
    Entity* zombie{ADD_ENTITY(Util::StrFormat("Zombie (%d)", i))};
    zombie->AddComponent<Zombie, true>();
    zombie->SetActive(false);
    zombies.push_back(zombie);
  }
}

void GameManager::Update() {
  cooldown -= Time::GetDeltaTime();
  if (cooldown <= 0) {
    SpawnZombie();
    cooldown = spawnInterval;
  }
}

void GameManager::GuiUpdate() {
  float base = 230;
  float padding = 20;
  GUI::SliderFloat(RectTransform{Math::Rect{-200, base, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Zombie Interval", &spawnInterval, 0, 5.f);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + padding, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Zombie Speed", &Zombie::speed, 0, 5.f);

  GUI::Text(RectTransform{Math::Rect{50, 200, 300, 100}, GUI::Pivot::Top,
                          GUI::Pivot::Top},
            Util::StrFormat("Score: %d!", score),
            GUI::TextStyle{Color::green, font});
}

void GameManager::SpawnZombie() const {
  auto player = PlayerController::Instance();
  if (player == nullptr) return;

  float angle = Math::Random::GetRandom01() * Math::Util::PI * 2;
  Math::Vector3 zombiePos =
      player->GetTransform()->GetWorldPos() +
      Math::Vector3{spawnRadius * Math::Util::Sin(angle), 0,
                    spawnRadius * Math::Util::Cos(angle)};

  Entity* zombie = nullptr;
  for (Size i = 0; i < zombies.size(); i++) {
    if (!zombies[i]->GetActive()) {
      zombie = zombies[i];
      break;
    }
  }

  if (zombie != nullptr) {
    zombie->SetActive(true);
    zombie->SetTransform(zombiePos, Math::Vector3::zero,
                         Math::Vector3::one * 0.02f);
  }
}
}  // namespace Isetta
