#include "Laser.h"

#include <iostream>
#include <SDL/SDL.h>

#include <Pengine/ResourceManager.h>


Laser::Laser(int id, glm::vec2 position, Pengine::ColorRGBA8 color, int type, Pengine::SoundEffect boxLandingSoundEffect)
{
	_id = id;
	_position = position;
	_color = color;
	_falling = false;
	_fallSteps = 0;
	_slidingLeft = false;
	_slideLeftSteps = 0;
	_slidingRight = false;
	_slideRightSteps = 0;
	_breathing = 0;
	_checked = false;
	_type = type;
	_boxType = BOX_TYPE::LASER_TYPE;
	_boxLandingSoundEffect = boxLandingSoundEffect;
	_textureID = Pengine::ResourceManager::getTexture("../Textures/laser.png").id;
}


Laser::~Laser()
{
}


bool Laser::update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) {
	// get the updated position 
	// cehck the fall steps
	if (_falling) {
		fall();
	}
	// check the sliding left steps
	if (_slidingLeft) {
		slideLeft();
	}
	// check the sliding right steps
	if (_slidingRight) {
		slideRight();
	}

	// check if breathing animation has stoped
	if (_breathing == 0) {
		// reset the texture
		_textureID = Pengine::ResourceManager::getTexture("../Textures/laser.png").id;
	}
	else {
		_breathing--;
	}

	if (!_highlighted) {
		// reset the texture
		_textureID = Pengine::ResourceManager::getTexture("../Textures/laser.png").id;
	}

	// check if there was a click on this Laser
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {

		// get the coordinates of the click
		glm::vec2 mouseCoords = inputManager.getMouseCoords();
		// convert screen coordinates to world coordinates to make the click
		mouseCoords = camera->convertScreenToWorld(mouseCoords);

		// check if the click was inside the Laser
		if (mouseCoords.x > _position.x && mouseCoords.x < _position.x + TILE_WIDTH &&
			mouseCoords.y > _position.y && mouseCoords.y < _position.y + TILE_WIDTH) {

			// there was a click on this Laser, thus delete it
			return true;
		}
	}


	// there was no click on this Laser
	return false;
}


void Laser::breatheAnimation() {
	// set the texture to breathing
	_textureID = Pengine::ResourceManager::getTexture("../Textures/laser_breathing.png").id;
}

void Laser::highlight() {
	// set the texture to breathing
	_textureID = Pengine::ResourceManager::getTexture("../Textures/laser_angry.png").id;
}

void Laser::resetTextureID() {

	// reset the texture
	_textureID = Pengine::ResourceManager::getTexture("../Textures/laser.png").id;
}