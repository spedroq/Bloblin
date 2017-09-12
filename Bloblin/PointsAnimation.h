#pragma once

#include <Pengine/SpriteFont.h>

#include <glm/glm.hpp>
#include <string>

const int POINTS_ANIMATION_LIFESPAN = 180;

const int POINTS_FONT_RES = 64;

class PointsAnimation
{
public:
	PointsAnimation(glm::vec2 position, std::string text, Pengine::ColorRGBA8 color);
	~PointsAnimation();

	// returns true if animation has finished
	bool update();

	// draw the points animation
	void draw(Pengine::SpriteBatch& pointsSpriteBatch);

protected:
	// sprite font
	Pengine::SpriteFont* _spriteFont;

	Pengine::ColorRGBA8 _color;

	// position
	glm::vec2 _position;
	// direction
	glm::vec2 _direction;
	// speed
	int _speed;

	// text
	std::string _text;

	// current life
	int _life;

};

