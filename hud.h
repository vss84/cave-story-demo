#pragma once

#include "sprite.h"
#include "player.h"

class Graphics;

class HUD
{
public:
	HUD();
	HUD(Graphics& graphics, Player& player);

	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

private:
	Player player_;
	// health sprites
	Sprite healthBarSprite_;
	Sprite healthNumberOne_;
	Sprite currentHealthBar_;
	// exp sprites
	Sprite levelText_;
	Sprite currentLevel_;
	Sprite expBar_;
	// weapon sprites
	Sprite slash_;
	Sprite dash_;
};