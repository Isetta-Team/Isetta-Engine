/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace Isetta {
class Font;
}  // namespace Isetta

namespace KnightGame {
DEFINE_COMPONENT(ScoreManager, Component, true)
private:
int score = 0;
Font *titleFont, *scoreFont;
static inline ScoreManager* instance;

int gameOverEvent;
bool gameOver = false;
float elapsed = 0;
const float speed = 0.4f;

public:
ScoreManager() = default;

static inline ScoreManager* Instance() { return instance; }

void Start() override;
void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;

inline void AddPoint() { score++; }
};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(ScoreManager, Component, true)