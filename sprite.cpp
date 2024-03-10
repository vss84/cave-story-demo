#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	x_(posX), y_(posY)
{
	this->sourceRect_.x = sourceX;
	this->sourceRect_.y = sourceY;
	this->sourceRect_.w = width;
	this->sourceRect_.h = height;

	this->spriteSheet_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->spriteSheet_ == NULL)
	{
		printf("\nError: Unable to load image\n");
	}

	this->boundingBox_ = Rectangle(this->x_, this->y_, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics& graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->sourceRect_.w * globals::SPRITE_SCALE,
											this->sourceRect_.h * globals::SPRITE_SCALE };
	graphics.blitSurface(this->spriteSheet_, &this->sourceRect_, &destinationRectangle);
}

void Sprite::update() 
{
	this->boundingBox_ = Rectangle(this->x_, this->y_, this->sourceRect_.w * globals::SPRITE_SCALE, this->sourceRect_.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const
{
	return this->boundingBox_;
}

const sides::Side Sprite::getCollisionSide(Rectangle& other) const
{
	int amountRight, amountLeft, amountTop, amountBottom;
	amountRight = this->getBoundingBox().getRight() - other.getLeft();
	amountLeft = other.getRight() - this->getBoundingBox().getLeft();
	amountTop = other.getBottom() - this->getBoundingBox().getTop();
	amountBottom = this->getBoundingBox().getBottom() - other.getTop();

	int values[4] = { abs(amountRight), abs(amountLeft), abs(amountTop), abs(amountBottom) };
	auto minElement = std::min_element(std::begin(values), std::end(values));
	
	return	*minElement == abs(amountRight) ? sides::RIGHT :
			*minElement == abs(amountLeft) ? sides::LEFT :
			*minElement == abs(amountBottom) ? sides::BOTTOM :
			*minElement == abs(amountTop) ? sides::TOP : sides::NONE;
	
}

void Sprite::setSourceRectX(int value)
{
	this->sourceRect_.x = value;
}
void Sprite::setSourceRectY(int value)
{
	this->sourceRect_.y = value;
}
void Sprite::setSourceRectW(int value)
{
	this->sourceRect_.w = value;
}
void Sprite::setSourceRectH(int value)
{
	this->sourceRect_.h = value;
}