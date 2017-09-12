#pragma once

#include "Menu.h"

#include <Pengine/Vertex.h>
#include <Pengine/Camera2D.h>

#include <Pengine/SpriteFont.h>

const int SCORE_X = 508;
const int SCORE_Y = 290;

// ManuGameOver class derived from Menu
class MenuGameOver : public Menu
{
public:
	MenuGameOver(int score,
				int screenWidth,
				int screenHeight,
				Pengine::Camera2D camera,
				Pengine::ColorRGBA8 retry,
				Pengine::ColorRGBA8 back,
				Pengine::SoundEffect clickSoundEffect,
				Pengine::SoundEffect gameOverSoundEffect);

	~MenuGameOver();

	// returns menu flags
	unsigned int update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) override;

	void draw() override;

protected:

	// sprite batch for the score text
	Pengine::SpriteBatch _scoreSpriteBatch;

	// sprite font
	Pengine::SpriteFont* _spriteFont;

	// level color
	Pengine::ColorRGBA8 _retryColor;
	// level color
	Pengine::ColorRGBA8 _backColor;

	// init the buttons
	void initButtons();
};

