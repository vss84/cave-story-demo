#pragma once

namespace globals
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 512;

	const float SPRITE_SCALE = 2.0f;
}

namespace sides
{
	enum Side
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	const inline Side getOppositeSide(Side side)
	{
		return side == TOP ? BOTTOM :
			   side == BOTTOM ? TOP :
			   side == LEFT ? RIGHT :
			   side == RIGHT ? LEFT : NONE;
	}
}
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct Vector2
{
	int x, y;
	
	Vector2() : x(0), y(0) {}
	
	Vector2(int x, int y) : x(x), y(y) {}
	
	Vector2 zero()
	{
		return Vector2(0, 0);
	}
};

