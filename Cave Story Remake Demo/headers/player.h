#pragma once
#include "animated_sprite.h"
#include "globals.h"
#include "slope.h"
#include "level.h"
#include "enemy.h"

class Graphics;

class Player : public AnimatedSprite
{
public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	void moveLeft();
	void moveRight();
	void stopMoving();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();
	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle>& others);
	void handleSlopeCollisions(std::vector<Slope>& others);
	void handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics);
	void handleEnemyCollisions(std::vector<Enemy*>& others);

	const float getX() const;
	const float getY() const;
	const inline int getMaxHealth() const { return this->maxHealth_; }
	const inline int getCurrentHealth() const { return this->currentHealth_; }

	void gainHealth(int amount);

private:
	float dx_, dy_;
	Direction facing_;
	bool grounded_;
	bool lookingUp_;
	bool lookingDown_;
	int maxHealth_;
	int currentHealth_;
};