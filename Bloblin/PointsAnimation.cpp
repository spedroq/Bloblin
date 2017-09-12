#include "PointsAnimation.h"


PointsAnimation::PointsAnimation(glm::vec2 position, std::string text, Pengine::ColorRGBA8 color) :
	_color(color),
	_position(position),
	_text(text),
	_direction(glm::vec2(0, 1)),
	_speed(5),
	_life(POINTS_ANIMATION_LIFESPAN)
{
	_spriteFont = new Pengine::SpriteFont("../Fonts/Baloo-Regular.ttf", POINTS_FONT_RES);
}


PointsAnimation::~PointsAnimation()
{
	// delete the sprite font
	delete _spriteFont;
}


bool PointsAnimation::update() {
	// update the position of the points animation
	_position += (_direction * glm::vec2(_speed));

	// update the life
	_life--;
	// check if animation is finished
	if (_life == 0) {
		// return true since animation is finished
		return true;
	}
	else {
		// return false since animation is not finished
		return false;
	}
}


void PointsAnimation::draw(Pengine::SpriteBatch& pointsSpriteBatch) {

	int numDigits = _text.size();




	// draw the score
	_spriteFont->draw(pointsSpriteBatch,
		_text.c_str(),
		_position,
		glm::vec2(1),
		1.0f,
		_color);
}