#pragma once

#include <Pengine/Vertex.h>
#include <Pengine/SpriteBatch.h>
#include <Pengine/SpriteFont.h>
#include <Pengine/ResourceManager.h>
#include <Pengine/InputManager.h>
#include <Pengine/Camera2D.h>
#include <Pengine/AudioEngine.h>

#include <glm/glm.hpp>
#include <string>

// button dimentions
const int BUTTON_WIDTH = 380;
const int BUTTON_HEIGHT = 130;

// buttons start position
const int BUTTONS_X = 315;
const int BUTTONS_Y = 110;
// gab between buttons
const int BUTTONS_GAP = 10;

// font resolution
const int FONT_RES = 64;

// give them bitwise values that can be passed together
enum ButtonFlags { 
	PLAY = 1,
	EXIT,
	RETRY,
	BACK,
	CONTINUE
};

class Button
{
public:
	Button(glm::vec2 position, Pengine::ColorRGBA8 color, std::string text, Pengine::SpriteBatch& buttonSpriteBatch, Pengine::SoundEffect clickSoundEffect);
	~Button();

	// returns true if it has been clicked
	bool Button::update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera);

private:
	Pengine::SoundEffect _clickSoundEffect;

	Pengine::SpriteFont* _spriteFont;

	// button position
	glm::vec2 _position;

	// button color
	Pengine::ColorRGBA8 _color;

	// color texture
	std::string _text;

	// texture
	int _textureID;
};

