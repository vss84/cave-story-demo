#include "animated_sprite.h"
#include "graphics.h"
#include "sprite.h"

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	frameIndex_(0),
	timeToUpdate_(timeToUpdate),
	visible_(true),
	currentAnimationOnce_(false),
	currentAnimation_("") {}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset)
{
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++)
	{
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->animations_.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->offsets_.insert(std::pair<std::string, Vector2>(name, offset));
}

void  AnimatedSprite::resetAnimations()
{
	this->animations_.clear();
	this->offsets_.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once)
{
	this->currentAnimationOnce_ = once;
	if (this->currentAnimation_ != animation)
	{
		this->currentAnimation_ = animation;
		this->frameIndex_ = 0;
	}
}

void AnimatedSprite::setVisible(bool visible)
{
	this->visible_ = visible;
}

void AnimatedSprite::stopAnimation()
{
	this->frameIndex_ = 0;
	this->animationDone(this->currentAnimation_);
}

void AnimatedSprite::update(int elapsedTime)
{
	Sprite::update();

	this->elapsedTime_ += elapsedTime;
	if (this->elapsedTime_ > this->timeToUpdate_)
	{
		this->elapsedTime_ -= this->timeToUpdate_;

		if (this->frameIndex_ < this->animations_[this->currentAnimation_].size() - 1)
		{
			this->frameIndex_++;
		}
		else
		{
			if (this->currentAnimationOnce_ == true)
			{
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics& graphics, int x, int y)
{
	if (this->visible_)
	{
		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->offsets_[this->currentAnimation_].x;
		destinationRectangle.y = y + this->offsets_[this->currentAnimation_].y;
		destinationRectangle.w = this->sourceRect_.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->sourceRect_.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->animations_[this->currentAnimation_][this->frameIndex_];
		graphics.blitSurface(this->spriteSheet_, &sourceRect, &destinationRectangle);
	}
}
