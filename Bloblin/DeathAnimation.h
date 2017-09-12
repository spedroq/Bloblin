#pragma once

#include <Pengine/SpriteBatch.h>

// number of frames it takes to do the death animation
const float DEATH_ANIMATION_FRAMES = 8.0f;

class DeathAnimation
{
public:
	DeathAnimation(Pengine::ColorRGBA8 color, glm::vec2 position);
	~DeathAnimation();

	// returns true if animation is finished
	bool update();

	// draw the death animation
	void draw(Pengine::SpriteBatch& deathSpriteBatch);

protected:
	// death animation colour
	Pengine::ColorRGBA8 _color;

	// position of the death animation
	glm::vec2 _position;

	// texture id
	int _textureID;

	// current animation step
	int _currentStep;

};

