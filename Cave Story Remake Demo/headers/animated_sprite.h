#pragma once

#include <map>
#include <string>
#include <vector>

#include "sprite.h"
#include "globals.h"

class Graphics;

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);

	void playAnimation(std::string animation, bool once = false);

	void update(int elapsedTime);

	void draw(Graphics& graphics, int x, int y);


protected:
	double timeToUpdate_;
	bool currentAnimationOnce_;
	std::string currentAnimation_;

	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	void resetAnimations();

	void stopAnimation();

	void setVisible(bool visible);

	virtual void animationDone(std::string currentAnimation) = 0;
	
	virtual void setupAnimations() = 0;

private:
	std::map<std::string, std::vector<SDL_Rect>> animations_;
	std::map<std::string, Vector2> offsets_;

	int frameIndex_;
	double elapsedTime_ = 0;
	bool visible_;

};