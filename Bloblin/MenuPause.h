#pragma once

#include "Menu.h"

#include <Pengine/Vertex.h>
#include <Pengine/Camera2D.h>

#include <Pengine/SpriteFont.h>

// MenuPause class derived from Menu
class MenuPause : public Menu
{
public:
	MenuPause(int screenWidth,
		int screenHeight,
		Pengine::Camera2D camera,
		Pengine::ColorRGBA8 continueGame,
		Pengine::ColorRGBA8 back,
		Pengine::SoundEffect clickSoundEffect);
	~MenuPause();

	// returns menu flags
	unsigned int update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void draw() override;

protected:

	// level color
	Pengine::ColorRGBA8 _continueColor;
	// level color
	Pengine::ColorRGBA8 _backColor;

	// init the buttons
	void initButtons();
};