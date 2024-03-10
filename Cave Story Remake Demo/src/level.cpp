#include <SDL.h>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "bat.h"
#include "utils.h"
#include "level.h"
#include "enemy.h"
#include "player.h"
#include "globals.h"
#include "graphics.h"
#include "tinyxml2.h"

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics) :
	mapName_(mapName), size_(size_.zero())
{
	this->loadMap(mapName, graphics);
}

Level::~Level() 
{

}

void Level::loadMap(std::string mapName, Graphics& graphics)
{
	// parse tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	// get width and height of map into size_
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->size_ = Vector2(width, height);

	// get width and height of tiles into tilesize_
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->tileSize_ = Vector2(tileWidth, tileHeight);

	// load tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL)
	{
		while (pTileset)
		{
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source"); // TODO(vss): add dynamic path editing to change "../tilesets/PrtCave.tsx" to "content/tilesets/PrtCave.png"
			std::stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->tileSets_.push_back(Tileset(texture, firstgid));

			// get the animations
			XMLElement* pTileAnimation = pTileset->FirstChildElement("tile");
			if (pTileAnimation != NULL)
			{
				while (pTileAnimation)
				{
					AnimatedTileInfo ati;
					ati.StartTileId = pTileAnimation->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement* pAnimation = pTileAnimation->FirstChildElement("animation");
					if (pAnimation != NULL)
					{
						while (pAnimation)
						{
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL)
							{
								while (pFrame)
								{
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->animatedTileInfos_.push_back(ati);
					pTileAnimation = pTileAnimation->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// load layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL)
	{
		while (pLayer)
		{
			// loading data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL)
			{
				while (pData)
				{
					// loading tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL)
					{
						int tileCounter = 0;
						while (pTile)
						{
							// build each tile
							// if gid = 0, don't draw a tile
							if (pTile->IntAttribute("gid") == 0)
							{
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}
							// get tileset for this gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->tileSets_.size(); i++)
							{
								if (this->tileSets_[i].FirstGid <= gid)
								{
									if (tileSets_[i].FirstGid > closest)
									{
										closest = this->tileSets_[i].FirstGid;
										tls = this->tileSets_.at(i);
									}
								}
							}

							if (tls.FirstGid == -1)
							{
								// no tileset found
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							// get the tile position in the level
							int x = 0;
							int y = 0;
							x = tileCounter % width;
							x *= tileWidth;
							y += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(x, y);
							
							// calculate tile position in tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							// build tile and add to level tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->animatedTileInfos_.size(); i++)
							{
								if (this->animatedTileInfos_.at(i).StartTileId == gid)
								{
									ati = this->animatedTileInfos_.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile)
							{
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++)
								{
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->animatedTileList_.push_back(tile);
							}
							else
							{
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
								this->tileList_.push_back(tile);
							}
							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	// parse collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL)
	{
		while (pObjectGroup)
		{
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->collisionRects_.push_back(Rectangle(
							std::ceil(x)* globals::SPRITE_SCALE,
							std::ceil(y)* globals::SPRITE_SCALE,
							std::ceil(width)* globals::SPRITE_SCALE,
							std::ceil(height)* globals::SPRITE_SCALE));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "slopes")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL)
						{
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");
							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							// loop through list of splitted pairs, split them into Vector2's
							// and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++)
							{
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2)
						{
							
							this->slopes_.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),

								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "spawnpoints")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player")
						{
							this->spawnPoint_ = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
														std::ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "doors")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL)
						{
							while (pProperties)
							{
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL)
								{
									while (pProperty)
									{
										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "destination")
										{
											const char* value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->doorList_.push_back(door);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "enemies")
			{
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat")
						{
							this->enemies_.push_back(new Bat(graphics,Vector2(std::floor(x)* globals::SPRITE_SCALE,
																std::floor(y)* globals::SPRITE_SCALE)));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime, Player& player)
{
	for (int i = 0; i < this->animatedTileList_.size(); i++)
	{
		this->animatedTileList_.at(i).update(elapsedTime);
	}
	for (int i = 0; i < this->enemies_.size(); i++)
	{
		this->enemies_.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics& graphics)
{
	for (int i = 0; i < this->tileList_.size(); i++)
	{
		this->tileList_.at(i).draw(graphics);
	}
	for (int i = 0; i < this->animatedTileList_.size(); i++)
	{
		this->animatedTileList_.at(i).draw(graphics);
	}
	for (int i = 0; i < this->enemies_.size(); i++)
	{
		this->enemies_.at(i)->draw(graphics);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->collisionRects_.size(); i++)
	{
		if (this->collisionRects_.at(i).collidesWith(other))
		{
			others.push_back(this->collisionRects_.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle& other)
{
	std::vector<Slope> others;
	for (int i = 0; i < this->slopes_.size(); i++)
	{
		if (this->slopes_.at(i).collidesWith(other))
		{
			others.push_back(this->slopes_.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle& other)
{
	std::vector<Door> others;
	for (int i = 0; i < this->doorList_.size(); i++)
	{
		if (this->doorList_.at(i).collidesWith(other))
		{
			others.push_back(this->doorList_.at(i));
		}
	}
	return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle& other)
{
	std::vector<Enemy*> others;
	for (int i = 0; i < this->enemies_.size(); i++)
	{
		if (this->enemies_.at(i)->getBoundingBox().collidesWith(other))
		{
			others.push_back(this->enemies_.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const
{
	return this->spawnPoint_;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight)
{
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsx = gid % (tilesetWidth / tileWidth) - 1;
	tsx *= tileWidth;
	int tsy = 0;
	int amount = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsy = tileHeight * amount;
	Vector2 finalTilesetPosition = Vector2(tsx, tsy);
	return finalTilesetPosition;
}