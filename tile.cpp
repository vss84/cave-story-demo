#include <SDL.h>
#include "tile.h"
#include "graphics.h"

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	tileset_(tileset),
	size_(size),
	tilesetPosition_(tilesetPosition),
	position_(Vector2(position.x* globals::SPRITE_SCALE, position.y* globals::SPRITE_SCALE)) {}

void Tile::update(int elapsedTime)
{
	
}

void Tile::draw(Graphics& graphics)
{
	SDL_Rect destRect = { this->position_.x, this->position_.y, this->size_.x * globals::SPRITE_SCALE, this->size_.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->tilesetPosition_.x, this->tilesetPosition_.y, this->size_.y, this->size_.y };

	graphics.blitSurface(this->tileset_, &sourceRect, &destRect);
}