#pragma once

#include "enemy.h"

class Bat : public Enemy
{
public:
	Bat();
	Bat(Graphics& graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);
	void touchPlayer(Player* player);
	void animationDone(std::string currentAnimation);
	void setupAnimations();

private:
	float startingX_, startingY_;
	bool shouldMoveUp_;
};

