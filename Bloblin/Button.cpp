#include "Button.h"

#include <SDL/SDL.h>

Button::Button(glm::vec2 position, Pengine::ColorRGBA8 color, std::string text, Pengine::SpriteBatch& buttonSpriteBatch, Pengine::SoundEffect clickSoundEffect) :
	_position(position),
	_color(color),
	_text(text),
	_textureID(Pengine::ResourceManager::getTexture("../Textures/button.png").id)
{
	// set the click sound effect
	_clickSoundEffect = clickSoundEffect;

	_spriteFont = new Pengine::SpriteFont("../Fonts/Baloo-Regular.ttf", FONT_RES);

	// set the uvRect
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	// set the destRect
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = BUTTON_WIDTH;
	destRect.w = BUTTON_HEIGHT;

	// draw the box
	buttonSpriteBatch.draw(destRect, uvRect, _textureID, 1.0f, _color);

	int numDigits = _text.size();

	// centre the text
	float positionX = (_position.x + (BUTTON_WIDTH / 2) - BUTTONS_GAP) - (numDigits / 2 * (FONT_RES / 2));
	float positionY = _position.y + BUTTONS_GAP;

	// draw the score
	_spriteFont->draw(buttonSpriteBatch,
		_text.c_str(),
		glm::vec2(positionX, positionY),
		glm::vec2(1),
		1.0f,
		_color);
}


Button::~Button()
{
}

// update button and check if it has been clicked
bool Button::update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) {

	// check if there was a click on this blob
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {

		// get the coordinates of the click
		glm::vec2 mouseCoords = inputManager.getMouseCoords();
		// convert screen coordinates to world coordinates to make the click
		mouseCoords = camera->convertScreenToWorld(mouseCoords);

		// check if the click was inside the blob
		if (mouseCoords.x > _position.x && mouseCoords.x < _position.x + BUTTON_WIDTH &&
			mouseCoords.y > _position.y && mouseCoords.y < _position.y + BUTTON_HEIGHT) {

			// play click sound effect
			_clickSoundEffect.play(0, 6);

			// there was a click on this blob, thus delete it
			return true;
		}
	}

	// there was no click on this blob
	return false;

}