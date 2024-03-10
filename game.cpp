#include <SDL.h>
#include "game.h"
#include "graphics.h"
#include "input.h"

namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game()
{

}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	SDL_Event event;
	this->level_ = Level("Map1", graphics);
	this->player_ = Player(graphics, this->level_.getPlayerSpawnPoint());
	this->hud_ = HUD(graphics, player_);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true)
	{
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.repeat == 0)
				{
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
		{
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true)
		{
			this->player_.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true)
		{
			this->player_.moveRight();
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP))
		{
			this->player_.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN))
		{
			this->player_.lookDown();
		}

		if (input.wasKeyReleased(SDL_SCANCODE_UP))
		{
			this->player_.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN))
		{
			this->player_.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Z) == true)
		{
			this->player_.jump();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT))
		{
			this->player_.stopMoving();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		this->graphics_ = graphics;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));

		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();
	this->level_.draw(graphics);
	this->player_.draw(graphics);
	this->hud_.draw(graphics);
	graphics.flip();
}

void Game::update(float elapsedTime)
{
	this->player_.update(elapsedTime);
	this->level_.update(elapsedTime, this->player_);
	this->hud_.update(elapsedTime, this->player_);
	// check collisions
	std::vector<Rectangle> others;
	if ((others = this->level_.checkTileCollisions(this->player_.getBoundingBox())).size() > 0)
	{
		// handle player collision
		this->player_.handleTileCollisions(others);
	}
	// check slopes
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->level_.checkSlopeCollisions(this->player_.getBoundingBox())).size() > 0)
	{
		this->player_.handleSlopeCollisions(otherSlopes);
	}
	// check doors
	std::vector<Door> otherDoors;
	if ((otherDoors = this->level_.checkDoorCollisions(this->player_.getBoundingBox())).size() > 0);
	{
		this->player_.handleDoorCollision(otherDoors, this->level_, this->graphics_);
	}
	// check enemies
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->level_.checkEnemyCollisions(this->player_.getBoundingBox())).size() > 0)
	{
		this->player_.handleEnemyCollisions(otherEnemies);
	}
}
