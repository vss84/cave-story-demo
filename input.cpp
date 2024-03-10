#include "input.h"

void Input::beginNewFrame()
{
	this->pressedKeys_.clear();
	this->releasedKeys_.clear();
}
void Input::keyDownEvent(const SDL_Event& event)
{
	this->pressedKeys_[event.key.keysym.scancode] = true;
	this->heldKeys_[event.key.keysym.scancode] = true;
}
void Input::keyUpEvent(const SDL_Event& event)
{
	this->releasedKeys_[event.key.keysym.scancode] = true;
	this->heldKeys_[event.key.keysym.scancode] = false;
}
bool Input::wasKeyPressed(SDL_Scancode key)
{
	return this->pressedKeys_[key];
}
bool Input::wasKeyReleased(SDL_Scancode key)
{
	return this->releasedKeys_[key];
}
bool Input::isKeyHeld(SDL_Scancode key)
{
	return this->heldKeys_[key];
}