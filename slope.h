#pragma once

#include <cmath>
#include "globals.h"
#include "rectangle.h"

class Slope
{
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) : p1_(p1), p2_(p2)
	{
		if (this->p2_.x - this->p1_.x != 0) 
		{
			this->slope_ = (fabs(this->p2_.y) - fabs(this->p1_.y)) / (fabs(this->p2_.x) - fabs(this->p1_.x));
		}
	}

	const inline float getSlope() const
	{
		return this->slope_;
	}

	const bool collidesWith(const Rectangle& other) const
	{
		return (other.getRight() >= this->p2_.x &&
				other.getLeft() <= this->p1_.x &&
				other.getTop() <= this->p2_.y &&
				other.getBottom() >= this->p1_.y) ||
			   (other.getRight() >= this->p1_.x &&
				other.getLeft() <= this->p2_.x &&
				other.getTop() <= this->p1_.y &&
				other.getBottom() >= this->p2_.y) ||

			   (other.getLeft() <= this->p1_.x &&
				other.getRight() >= this->p2_.x &&
				other.getTop() <= this->p1_.y &&
				other.getBottom() >= this->p2_.y) ||
			   (other.getLeft() <= this->p2_.x &&
				other.getRight() >= this->p1_.x &&
				other.getTop() <= this->p2_.y &&
				other.getBottom() >= this->p1_.y);
	}

	const inline Vector2 getP1() const { return this->p1_; }
	const inline Vector2 getP2() const { return this->p2_; }

private:
	Vector2 p1_, p2_;
	float slope_;
};	