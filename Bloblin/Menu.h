#pragma once

#include <Pengine/SpriteBatch.h>
#include <Pengine/AudioEngine.h>

#include "Button.h"



class Menu
{
public:
	Menu();
	virtual ~Menu();

	// update the menu
	virtual unsigned int update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) = 0;

	// draw the menu
	virtual void draw() = 0;

protected:
	// sprite batch for the background
	Pengine::SpriteBatch _spriteBatch;

	// sprite batch for the buttons
	Pengine::SpriteBatch _buttonSpriteBatch;

	// sound effect
	Pengine::SoundEffect _clickSoundEffect;

	// list of buttons
	std::vector<Button> _buttons;

	// background texture
	int _textureID;
};



