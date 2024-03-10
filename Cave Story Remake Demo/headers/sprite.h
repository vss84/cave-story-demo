#pragma once
#include <SDL.h>
#include <string>
#include "rectangle.h"
#include "globals.h"

class Graphics;

class Sprite
{
public:
	Sprite();
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics& graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle& other) const;

	const inline float getX() const { return this->x_; }
	const inline float getY() const { return this->y_; }
	
	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

protected:
	SDL_Rect sourceRect_;
	SDL_Texture* spriteSheet_;
	Rectangle boundingBox_;
	float x_, y_;
	
};