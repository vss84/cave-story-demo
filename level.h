#pragma once

#include <string>
#include <vector>

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"
#include "animated_tile.h"
#include "door.h"


class Graphics;
class Enemy;
class Bat;
class Player;
struct Tileset;
struct SDL_Rect;
struct SDL_Texture;

class Level
{
public:
	Level();
	Level(std::string mapName, Graphics& graphics);
	~Level();
	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle& other);
	std::vector<Door> checkDoorCollisions(const Rectangle& other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle& other);
	const Vector2 getPlayerSpawnPoint() const;

private:
	std::string mapName_;
	Vector2 spawnPoint_;
	Vector2 size_;
	Vector2 tileSize_;
	SDL_Texture* backgroundTexture_;

	std::vector<Tile> tileList_;
	std::vector<Tileset> tileSets_;
	std::vector<Rectangle> collisionRects_;
	std::vector<Slope> slopes_;
	std::vector<Door> doorList_;

	std::vector<Enemy* > enemies_;

	std::vector<AnimatedTile> animatedTileList_;
	std::vector<AnimatedTileInfo> animatedTileInfos_;

	void loadMap(std::string mapName, Graphics& graphics);
	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

struct Tileset
{
	SDL_Texture* Texture;
	int FirstGid;

	Tileset()
	{
		this->FirstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid)
	{
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};