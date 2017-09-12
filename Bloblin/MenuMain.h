#pragma once

#include "Menu.h"

#include <Pengine/Vertex.h>
#include <Pengine/Camera2D.h>

#include <Pengine/SpriteFont.h>

// ManuMain class derived from Menu
class MenuMain : public Menu
{
public:
	MenuMain(int screenWidth,
			int screenHeight,
			Pengine::Camera2D camera,
			Pengine::ColorRGBA8 play,
			Pengine::ColorRGBA8 exit,
			Pengine::SoundEffect clickSoundEffect);
	~MenuMain();

	// returns menu flags
	unsigned int update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void draw() override;

protected:

	// level color
	Pengine::ColorRGBA8 _playColor;
	// level color
	Pengine::ColorRGBA8 _exitColor;

	// init the buttons
	void initButtons();
};