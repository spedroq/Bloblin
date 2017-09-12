#include "MenuGameOver.h"

#include <Pengine/ResourceManager.h>

MenuGameOver::MenuGameOver(int score,
							int screenWidth,
							int screenHeight,
							Pengine::Camera2D camera,
							Pengine::ColorRGBA8 retry,
							Pengine::ColorRGBA8 back,
							Pengine::SoundEffect clickSoundEffect,
							Pengine::SoundEffect gameOverSoundEffect) :
	_retryColor(retry),
	_backColor(back)
{
	// play the game over sound effect
	gameOverSoundEffect.play(0, 7);


	// get the click sound effect for the buttons
	_clickSoundEffect = clickSoundEffect;


	// init sprite font
	_spriteFont = new Pengine::SpriteFont("../Fonts/Baloo-Regular.ttf", FONT_RES);

	// initialize sprite batch
	_spriteBatch.init();
	// start sprite batch
	_spriteBatch.begin();

	// draw background
	Pengine::ColorRGBA8 white= Pengine::ColorRGBA8(255, 255, 255, 255);
	// get destRect
	glm::vec4 destRect(0, -74, (float) screenWidth / camera.getScale(), (float) screenHeight / camera.getScale());
	// get uvRect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	// draw the background
	_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/gameover_small.png").id, 1.0f, white);

	// end sprite batch
	_spriteBatch.end();
	_spriteBatch.renderBatch();


	// initialize sprite batch
	_scoreSpriteBatch.init();
	// start sprite batch
	_scoreSpriteBatch.begin();

	// display the score

	// buffer to hold the text to be rendered
	char buffer[256];

	// fill buffer with score data
	sprintf_s(buffer, "Score: %d", score);

	// centre the score text

	// count number of digits
	unsigned int numDigits = 0;
	do {
		++numDigits;
		score /= 10;
	} while (score);

	// count the letters in score
	int numLetters = numDigits + 7;

	// draw the score
	_spriteFont->draw(_scoreSpriteBatch,
						buffer,
						glm::vec2(SCORE_X - (numLetters/2 * (FONT_RES/2)), SCORE_Y),
						glm::vec2(1),
						1.0f,
						white);


	// end sprite batch
	_scoreSpriteBatch.end();
	_scoreSpriteBatch.renderBatch();

	// init the buttons
	initButtons();
}


MenuGameOver::~MenuGameOver()
{
	delete _spriteFont;
}

void MenuGameOver::initButtons() {

	// initialize sprite batch
	_buttonSpriteBatch.init();
	// start sprite batch
	_buttonSpriteBatch.begin();

	// make a retry button
	_buttons.push_back(*new Button(glm::vec2(BUTTONS_X, BUTTONS_Y),
						_retryColor,
						"Retry",
						_buttonSpriteBatch,
						_clickSoundEffect));

	// make a back button
	_buttons.push_back(*new Button(glm::vec2(BUTTONS_X, (BUTTONS_Y - (BUTTONS_GAP + BUTTON_HEIGHT))),
						_backColor,
						"Back",
						_buttonSpriteBatch,
						_clickSoundEffect));

	// end sprite batch
	_buttonSpriteBatch.end();
	_buttonSpriteBatch.renderBatch();
}

unsigned int MenuGameOver::update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) {
	// flags to send back to the main game
	unsigned int flags = 0;
	// update buttons
	if (_buttons[0].update(inputManager, camera)) {
		// take the retry bit
		flags = RETRY;
	}
	else if (_buttons[1].update(inputManager, camera)) {
		// take the back bit
		flags = BACK;
	}
	
	return flags;
}

void MenuGameOver::draw() {

	// render the batch of the background
	_spriteBatch.renderBatch();

	// render the score
	_scoreSpriteBatch.renderBatch();

	// render bacth of the buttons
	_buttonSpriteBatch.renderBatch();

}