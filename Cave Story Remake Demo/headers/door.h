#pragma once
#include <string>
#include "rectangle.h"
#include "globals.h"

class Door : public Rectangle
{
public:
	Door();
	Door(Rectangle rect, std::string destination) :
		Rectangle(rect.getLeft()* globals::SPRITE_SCALE, rect.getTop()* globals::SPRITE_SCALE,
			rect.getWidth()* globals::SPRITE_SCALE, rect.getHeight()* globals::SPRITE_SCALE),
			destination_(destination) {}

	const inline Rectangle getRectangle() const { return this->getRect(); }
	const inline std::string getDestination() const { return this->destination_; }

private:
	std::string destination_;
};