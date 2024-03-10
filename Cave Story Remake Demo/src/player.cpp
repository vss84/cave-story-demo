#include "player.h"
#include "graphics.h"

namespace player_constants
{
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	dx_(0), dy_(0), facing_(RIGHT), grounded_(false), lookingUp_(false), lookingDown_(false), maxHealth_(3), currentHealth_(3)
{
	graphics.loadImage("content/sprites/MyChar.png");

	this->setupAnimations();
	this->playAnimation("RunRight");
}

void Player::setupAnimations()
{
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackRight", 16, 16, Vector2(0, 0));
}

void Player::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->x_, this->y_);
}

void Player::moveLeft()
{
	if (this->lookingDown_ && this->grounded_)
	{
		return;
	}
	this->dx_ = -player_constants::WALK_SPEED;
	if (!this->lookingUp_)
	{
		this->playAnimation("RunLeft");
	}
	this->facing_ = LEFT;
}
	
void Player::moveRight()
{
	if (this->lookingDown_ && this->grounded_)
	{
		return;
	}
	this->dx_ = player_constants::WALK_SPEED;
	if (!this->lookingUp_)
	{
		this->playAnimation("RunRight");
	}
	this->facing_ = RIGHT;
}

void Player::stopMoving()
{
	this->dx_ = 0.0f;
	if (!this->lookingUp_ && !this->lookingDown_)
	{
		this->playAnimation(this->facing_ == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::lookUp()
{
	this->lookingUp_ = true;
	if (!this->dx_)
	{
		this->playAnimation(this->facing_ == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else 
	{
		this->playAnimation(this->facing_ == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp()
{
	this->lookingUp_ = false;
}

void Player::lookDown()
{
	this->lookingDown_ = true;
	if (this->grounded_)
	{
		dx_ = 0;
		// interact
		this->playAnimation(this->facing_ == RIGHT ? "LookBackRight" : "LookBackLeft");
	}
	else
	{
		this->playAnimation(this->facing_ == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown()
{
	this->lookingDown_ = false;
}

void Player::jump()
{
	if (this->grounded_)
	{
		this->dy_ = 0;
		this->dy_ -= player_constants::JUMP_SPEED;
		this->grounded_ = false;
	}
}

void Player::handleTileCollisions(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++)
	{
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE)
		{
			switch (collisionSide)
			{
			case sides::TOP:
				this->dy_ = 0;
				this->y_ = others.at(i).getBottom() + 1;
				if (this->grounded_)
				{
					this->dx_ = 0;
					this->x_ -= this->facing_ == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				this->y_ = others.at(i).getTop() - this->boundingBox_.getHeight() - 1;
				this->dy_ = 0;
				grounded_ = true;
				break;
			case sides::LEFT:
				this->x_ = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->x_ = others.at(i).getLeft() - this->boundingBox_.getWidth() - 1;
				break; 
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope>& others)
{
	for (int i = 0; i < others.size(); i++)
	{
		// calculate slope intercept
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));
		// players center x
		int centerX = this->boundingBox_.getCenterX();
		// plug x in y = mx+b for new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 8; // 8 is a temp magic number fix
		// re position player to correct y
		if (this->grounded_)
		{
			this->y_ = newY - this->boundingBox_.getHeight();
			this->grounded_ = true;
		}
	}	
}

void Player::handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics)
{
	for (int i = 0; i < others.size(); i++)
	{
		if (this->grounded_ && this->lookingDown_)
		{
			level = Level(others.at(i).getDestination(), graphics);
			this->x_ = level.getPlayerSpawnPoint().x;
			this->y_ = level.getPlayerSpawnPoint().y;
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*>& others)
{
	for (int i = 0; i < others.size(); i++)
	{
		others.at(i)->touchPlayer(this);
	}
}

void Player::gainHealth(int amount)
{
	this->currentHealth_ += amount;
}

void Player::update(float elapsedTime)
{
	// apply gravity
	if (this->dy_ <= player_constants::GRAVITY_CAP)
	{
		this->dy_ += player_constants::GRAVITY * elapsedTime;
	}
	// move by dx
	this->x_ += this->dx_ * elapsedTime;
	// move by dy
	this->y_ += this->dy_ * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const
{
	return this->x_;
}
const float Player::getY() const
{
	return this->y_;
}
