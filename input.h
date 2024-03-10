#pragma once

#include <map>
#include <SDL.h>

class Input
{
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

private:
	std::map<SDL_Scancode, bool> heldKeys_;
	std::map<SDL_Scancode, bool> pressedKeys_;
	std::map<SDL_Scancode, bool> releasedKeys_;
};