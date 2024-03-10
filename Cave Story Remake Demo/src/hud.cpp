#include "hud.h"
#include "graphics.h"

HUD::HUD() {}

HUD::HUD(Graphics& graphics, Player& player)
{
	this->player_ = player;
	this->healthBarSprite_ = Sprite(graphics, "content/sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->healthNumberOne_ = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	this->currentHealthBar_ = Sprite(graphics, "content/sprites/TextBox.png", 0, 25, 39, 5, 83, 72);
	this->levelText_ = Sprite(graphics, "content/sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
	this->currentLevel_ = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 52);
	this->expBar_ = Sprite(graphics, "content/sprites/TextBox.png", 0, 72, 40, 8, 83, 52);
	this->slash_ = Sprite(graphics, "content/sprites/TextBox.png", 72, 48, 8, 8, 100, 36);
	this->dash_ = Sprite(graphics, "content/sprites/TextBox.png", 81, 51, 15, 11, 132, 26);

}

void HUD::update(int elapsedTime, Player& player)
{
	this->player_ = player;
	this->healthNumberOne_.setSourceRectX(8 * player_.getCurrentHealth());

	// update health bar
	float num = (float)this->player_.getCurrentHealth() / this->player_.getMaxHealth();
	this->currentHealthBar_.setSourceRectW(std::floor(num * 39));
}

void HUD::draw(Graphics& graphics)
{
	this->healthBarSprite_.draw(graphics, this->healthBarSprite_.getX(), this->healthBarSprite_.getY());
	this->healthNumberOne_.draw(graphics, this->healthNumberOne_.getX(), this->healthNumberOne_.getY());
	this->currentHealthBar_.draw(graphics, this->currentHealthBar_.getX(), this->currentHealthBar_.getY());
	this->levelText_.draw(graphics, this->levelText_.getX(), this->levelText_.getY());
	this->currentLevel_.draw(graphics, this->currentLevel_.getX(), this->currentLevel_.getY());
	this->expBar_.draw(graphics, this->expBar_.getX(), this->expBar_.getY());
	this->slash_.draw(graphics, this->slash_.getX(), this->slash_.getY());
	this->dash_.draw(graphics, this->dash_.getX(), this->dash_.getY());
}