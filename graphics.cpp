#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "globals.h"

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->window_, &this->renderer_);
	SDL_SetWindowTitle(this->window_, "Cavestory Demo");
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(this->window_);
	SDL_DestroyRenderer(this->renderer_);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath)
{
	if (this->spriteSheets_.count(filePath) == 0)
	{
		this->spriteSheets_[filePath] = IMG_Load(filePath.c_str());
	}
	return this->spriteSheets_[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle)
{
	SDL_RenderCopy(this->renderer_, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(this->renderer_);
}

void Graphics::clear()
{
	SDL_RenderClear(this->renderer_);
}

SDL_Renderer* Graphics::getRenderer() const
{
	return this->renderer_;
}