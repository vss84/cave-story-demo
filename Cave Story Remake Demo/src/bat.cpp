#include "bat.h"

Bat::Bat() {}

Bat::Bat(Graphics& graphics, Vector2 spawnPoint) :
	Enemy(graphics, "content/sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	startingX_(spawnPoint.x),
	startingY_(spawnPoint.y),
	shouldMoveUp_(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player& player)
{
	this->direction_ = player.getX() > this->x_ ? RIGHT : LEFT;
	this->playAnimation(this->direction_ == RIGHT ? "FlyRight" : "FlyLeft");
	this->y_ += this->shouldMoveUp_ ? -.02f : .02f;
	if (this->y_ > (this->startingY_ + 20) || this->y_ < this->startingY_ - 20)
	{
		this->shouldMoveUp_ = !this->shouldMoveUp_;
	}
	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics& graphics)
{
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation)
{

}

void Bat::setupAnimations()
{
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player* player)
{
	player->gainHealth(-1);
}