#include "enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	direction_(LEFT),
	maxHealth_(0),
	currentHealth_(0) {}


void Enemy::update(int elapsedTime, Player& player)
{
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->x_, this->y_);
}

