#include "MenuMain.h"

#include <Pengine/ResourceManager.h>

MenuMain::MenuMain(int screenWidth,
					int screenHeight,
					Pengine::Camera2D camera,
					Pengine::ColorRGBA8 play,
					Pengine::ColorRGBA8 exit,
					Pengine::SoundEffect clickSoundEffect) :
	_playColor(play),
	_exitColor(exit)
{
	// get the click sound effect for the buttons
	_clickSoundEffect = clickSoundEffect;
	// initialize sprite batch
	_spriteBatch.init();
	// start sprite batch
	_spriteBatch.begin();

	// draw background
	Pengine::ColorRGBA8 white = Pengine::ColorRGBA8(255, 255, 255, 255);
	// get destRect
	glm::vec4 destRect(0, -74, (float)screenWidth / camera.getScale(), (float)screenHeight / camera.getScale());
	// get uvRect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	// draw the background
	_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/main_menu_small.png").id, 1.0f, white);


	// end sprite batch
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	// init the buttons
	initButtons();
}


MenuMain::~MenuMain()
{
}

void MenuMain::initButtons() {

	// initialize sprite batch
	_buttonSpriteBatch.init();
	// start sprite batch
	_buttonSpriteBatch.begin();

	// make a retry button
	_buttons.push_back(*new Button(glm::vec2(BUTTONS_X, BUTTONS_Y),
									_playColor,
									"Play",
									_buttonSpriteBatch,
									_clickSoundEffect));

	// make a back button
	_buttons.push_back(*new Button(glm::vec2(BUTTONS_X, (BUTTONS_Y - (BUTTONS_GAP + BUTTON_HEIGHT))),
									_exitColor,
									"Exit",
									_buttonSpriteBatch,
									_clickSoundEffect));

	// end sprite batch
	_buttonSpriteBatch.end();
	_buttonSpriteBatch.renderBatch();
}

unsigned int MenuMain::update(Pengine::InputManager& inputManager, Pengine::Camera2D* camera) {
	// flags to send back to the main game
	unsigned int flags = 0;
	// update buttons
	if (_buttons[0].update(inputManager, camera)) {
		// take the play bit
		flags = PLAY;
	}
	else if (_buttons[1].update(inputManager, camera)) {
		// take the exit bit
		flags = EXIT;
	}

	return flags;
}

void MenuMain::draw() {

	// render the batch of the background
	_spriteBatch.renderBatch();

	// render bacth of the buttons
	_buttonSpriteBatch.renderBatch();

}