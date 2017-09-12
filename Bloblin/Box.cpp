#include "Box.h"

#include <Pengine/ResourceManager.h>

#include <iostream>

Box::Box()
{
}


Box::~Box()
{
}

bool Box::slideRight() {
	// check if the sliding steps are not zero
	if (_slideRightSteps > 0) {
		// perform a sliding step
		_position.x += SLIDE_LEFT_STEP;
		_slideRightSteps--;
		_slidingRight = true;
	}
	else {
		// otherwise not falling anymore
		_slidingRight = false;
	}

	return _slidingRight;
}

bool Box::slideLeft() {
	// check if the sliding steps are not zero
	if (_slideLeftSteps > 0) {
		// perform a sliding step
		_position.x -= SLIDE_LEFT_STEP;
		_slideLeftSteps--;
		_slidingLeft = true;
	}
	else {
		// otherwise not falling anymore
		_slidingLeft = false;
	}

	return _slidingLeft;
}

bool Box::fall() {
	// check if the fall steps are not zero
	if (_fallSteps > 0) {
		// perform fall step
		_position.y -= FALL_STEP;
		_fallSteps--;
		_falling = true;

		// if we have just stoped falling
		if (_fallSteps == 0) {
			// do the land sound effect
			_boxLandingSoundEffect.play();
		}

	}
	else {
		// otherwise not falling anymore
		_falling = false;
	}

	return _falling;
}

int Box::outOfBoundsFall() {

	// fall
	_position.y -= FALL_STEP;

	return _outOfBoundsAnimationSteps--;


}

bool Box::checkHover(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) {

	// get the coordinates of the click
	glm::vec2 mouseCoords = inputManager.getMouseCoords();
	// convert screen coordinates to world coordinates to make the click
	mouseCoords = camera->convertScreenToWorld(mouseCoords);

	// check if the click was inside the blob
	if (mouseCoords.x > _position.x && mouseCoords.x < _position.x + TILE_WIDTH &&
		mouseCoords.y > _position.y && mouseCoords.y < _position.y + TILE_WIDTH) {

		// there was a click on this blob, thus delete it
		return true;
	}
	else {
		// there was no click on this blob
		return false;
	}
}

void Box::draw(Pengine::SpriteBatch& boxSpriteBatch) {
	
	// set the uvRect
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	// set the destRect
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = TILE_WIDTH;
	destRect.w = TILE_WIDTH;

	// draw the box
	boxSpriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);
}
