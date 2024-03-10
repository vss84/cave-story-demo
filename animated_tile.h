#pragma once

#include <vector>

#include "tile.h"
#include "globals.h"

class AnimatedTile : public Tile
{
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);

	void update(int elapsedTime);
	void draw(Graphics& graphics);

protected:
	int amountOfTime_ = 0;
	bool notDone_ = false;

private:
	std::vector<Vector2> tilesetPositions_;
	int tileToDraw_;
	int duration_;
};

struct AnimatedTileInfo
{
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	int Duration;
};