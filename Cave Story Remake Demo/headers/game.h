#pragma once

#include "player.h"
#include "level.h"
#include "hud.h"
#include "graphics.h"

class Game
{
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	Player player_;
	Level level_;
	HUD hud_;
	Graphics graphics_;
};