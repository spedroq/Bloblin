#include "DeathAnimation.h"

#include <Pengine/ResourceManager.h>

#include "Box.h"

DeathAnimation::DeathAnimation(Pengine::ColorRGBA8 color, glm::vec2 position)
{
	_color = color;
	_position = position;
	_currentStep = DEATH_ANIMATION_FRAMES;
	_textureID = Pengine::ResourceManager::getTexture("../Textures/blob_death_1.png").id;
}


DeathAnimation::~DeathAnimation()
{
}

// returns true if animation is finished
bool DeathAnimation::update() {

	if (_currentStep == 0) {
		return true;
	}
	else {
		// update the death sprite
		if (_currentStep == 6) {
			// change the death animation
			_textureID = Pengine::ResourceManager::getTexture("../Textures/blob_death_2.png").id;
		}
		else if (_currentStep == 4) {
			// change the death animation
			_textureID = Pengine::ResourceManager::getTexture("../Textures/blob_death_3.png").id;
		}
		else if (_currentStep == 4) {
			// change the death animation
			_textureID = Pengine::ResourceManager::getTexture("../Textures/blob_death_4.png").id;
		}
		// decrease the step counter
		_currentStep--;
		return false;
	}
}

// draw the death animation
void DeathAnimation::draw(Pengine::SpriteBatch& deathSpriteBatch) {
	// set the uvRect
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	// set the destRect
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = TILE_WIDTH;
	destRect.w = TILE_WIDTH;

	// draw the box
	deathSpriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);
}
