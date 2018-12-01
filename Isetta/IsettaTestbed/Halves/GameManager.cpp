/*
 * Copyright (c) 2018 Isetta
 */
#include "GameManager.h"

#include "PlayerController.h"
#include "Zombie.h"

namespace Isetta {
std::vector<Entity*> GameManager::zombies;
int GameManager::score = 0;

void GameManager::Start() {
  Font::AddFontFromFile("Halves/ZOMBIE.TTF", 48.0f, "Zombie");
}

void GameManager::OnEnable() {
  // create zombie pool
  // This is assuming game manager is only enabled once
  zombies.reserve(poolSize);
  for (int i = 0; i < poolSize; i++) {
    Entity* zombie{Entity::Instantiate(Util::StrFormat("Zombie (%d)", i))};
    zombie->AddComponent<Zombie, true>();
    zombie->SetActive(false);
    zombies.push_back(zombie);
  }
}

void GameManager::Update() {
  // zombie spawn logic
  cooldown -= Time::GetDeltaTime();
  if (cooldown <= 0) {
    SpawnZombie();
    cooldown = spawnInterval;
  }
}

void GameManager::GuiUpdate() {
  if (!font) font = Font::GetFont("Zombie", 48.f);

  // draw tweakable values
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
  // spawn a zombie
  auto player = PlayerController::Instance();
  if (player == nullptr) return;

  float angle = Math::Random::GetRandom01() * Math::Util::PI * 2;
  Math::Vector3 zombiePos =
      player->transform->GetWorldPos() +
      Math::Vector3{spawnRadius * Math::Util::Sin(angle), 0,
                    spawnRadius * Math::Util::Cos(angle)};

  Entity* zombie = nullptr;
  for (Size i = 0; i < zombies.size(); i++) {
    if (!zombies[i]->GetActive()) {
      zombie = zombies[i];
      break;
    }
  }

  // reanimate a zombie
  if (zombie != nullptr) {
    zombie->SetActive(true);
    zombie->SetTransform(zombiePos, Math::Vector3::zero,
                         Math::Vector3::one * 0.02f);
  }
}
}  // namespace Isetta
