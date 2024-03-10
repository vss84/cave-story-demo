#include "animated_tile.h"
#include "graphics.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position) :
	Tile(tileset, size, tilesetPositions.at(0), position),
	tilesetPositions_(tilesetPositions),
	duration_(duration),
	tileToDraw_(0) {}

void AnimatedTile::update(int elapsedTime)
{
	if (this->amountOfTime_ <= 0)
	{
		if (this->tileToDraw_ == this->tilesetPositions_.size() - 1)
		{
			this->tileToDraw_ = 0;
		}
		else
		{
			this->tileToDraw_++;
		}
		this->amountOfTime_ = this->duration_;
	}
	else
	{
		this->amountOfTime_ -= elapsedTime;
	}
	Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics& graphics)
{
	SDL_Rect destRect = { this->position_.x, this->position_.y, this->size_.x * globals::SPRITE_SCALE, this->size_.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->tilesetPositions_.at(this->tileToDraw_).x, this->tilesetPositions_.at(this->tileToDraw_).y,
							this->size_.x, this->size_.y };
	graphics.blitSurface(this->tileset_, &sourceRect, &destRect);
}