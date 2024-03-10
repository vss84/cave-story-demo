#pragma once

#include <string>
#include "animated_sprite.h"
#include "globals.h"
#include "player.h"

class Graphics;

class Enemy : public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player& player);
	virtual void draw(Graphics& graphics);
	virtual void touchPlayer(Player* player) = 0;

	const inline int getMaxHealth() const { return this->maxHealth_; }
	const inline int getCurrentHealth() const { return this->currentHealth_; }

protected:
	Direction direction_;
	int maxHealth_;
	int currentHealth_;
};

