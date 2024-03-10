#pragma once
#include <SDL.h>
#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
public:
	Graphics();
	~Graphics();

	SDL_Surface* loadImage(const std::string& filePath);
	void blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	void flip();
	void clear();
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	std::map<std::string, SDL_Surface*> spriteSheets_;
};