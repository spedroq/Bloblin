#include "MainGame.h"
#include <SDL/SDL.h>

#include <Pengine/Pengine.h>
#include <Pengine/Timing.h>

#include <random>
#include <ctime>
#include <iostream>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::MAIN_MENU),
	_fps(0),
	_score(0),
	_menu(nullptr)
{
}

MainGame::~MainGame(){
	// free up memory
	// delete level
	delete _level;
	// delete music
	delete _music;
	// delete menu
	delete _menu;
	// clear soundeffects
	_soundEffects.clear();
}

void MainGame::run() {
	
	// initialize all systems the game needs
	initSystems();

	// start the music
	Pengine::Music _music = _audioEngine.loadMusic("../Sound/Caketown.mp3");
	// play the music in a loop
	_music.play(-1);

	// init the game loop
	gameLoop();

}

void MainGame::initSystems() {

	// initialize game engine
	Pengine::init();

	// initialize sound (must happen after Pengine::init();
	_audioEngine.init();

	// load all the sounds
	loadSoundEffects();

	// initialize window
	_window.create("Bloblin Game", _screenWidth, _screenHeight, 0);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// initialize the shaders
	initShaders();

	// initialize the camera
	_camera.init(_screenWidth, _screenHeight);

	// set the camera starting in the middle of the screen adjusted to the background and box spawn
	_camera.setPosition(glm::vec2((float) ((_screenWidth / 2.0f)), (float) ((_screenHeight / 2.0f) - 74.0f)));
	// set camera scale
	_camera.setScale(CAMERA_SCALE);

	// create the main menu
	_menu = new MenuMain(_screenWidth,
						_screenHeight,
						_camera,
						Pengine::ColorRGBA8(0, 255, 0, 255),
						Pengine::ColorRGBA8(255, 0, 0, 255),
						_soundEffects[SoundEffects::MENU_CLICK]);
}

void MainGame::initShaders() {
    // compile our color shader
	_textureProgram.compileShaders("../Shaders/textureShading.vert", "../Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::loadSoundEffects() {
	// set the sound effect for the menu click
	_soundEffects[SoundEffects::MENU_CLICK] = _audioEngine.loadSoundEffect("../Sound/menu_button.ogg");
	// set the sound effect for the blob explosion
	_soundEffects[SoundEffects::BLOB_EXPLOSION] = _audioEngine.loadSoundEffect("../Sound/impact_splat.ogg");
	// set the sound effect for the box landing
	_soundEffects[SoundEffects::BOX_LANDING] = _audioEngine.loadSoundEffect("../Sound/jumpland.ogg");
	// set the sound effect for passing a level
	_soundEffects[SoundEffects::LEVEL_COMPLETE] = _audioEngine.loadSoundEffect("../Sound/level_complete.ogg");
	// set the sound effect for the box landing
	_soundEffects[SoundEffects::GAMEOVER] = _audioEngine.loadSoundEffect("../Sound/gameover.ogg");
	// set the sound effect for the pause menu
	_soundEffects[SoundEffects::PAUSE] = _audioEngine.loadSoundEffect("../Sound/clock_ticking.ogg");
	
}


void MainGame::updateMenu() {
	
	// check if there is a menu
	if (_menu != nullptr) {
		// update the menu and retrieve the flag
		unsigned int currentFlag = _menu->update(_inputManager, &_camera);

		if (currentFlag == PLAY) {
			// delete the main menu
			delete _menu;
			_menu = nullptr;
			_gameState = GameState::PLAY;
			// start a new game
			startNewGame();
		}
		else if (currentFlag == CONTINUE) {
			// delete the pause menu
			delete _menu;
			_menu = nullptr;
			// continue game
			_gameState = GameState::PLAY;
		}
		else if (currentFlag == EXIT) {
			// exit game
			_gameState = GameState::EXIT;
		}
		else if (currentFlag == RETRY) {
			// delete the gameover menu
			delete _menu;
			_menu = nullptr;
			// set game state to play
			_gameState = GameState::PLAY;
			// delete current level
			delete _level;
			_level = nullptr;
			// start new level
			startNewGame();
		}
		else if (currentFlag == BACK) {
			// delete the gameover menu
			delete _menu;
			_menu = nullptr;
			// set game state to play
			_gameState = GameState::MAIN_MENU;
			// delete current level
			delete _level;
			_level = nullptr;
			// go back to main menu
			// create the main menu
			_menu = new MenuMain(_screenWidth,
								_screenHeight,
								_camera,
								Pengine::ColorRGBA8(0, 255, 0, 255),
								Pengine::ColorRGBA8(255, 0, 0, 255),
								_soundEffects[SoundEffects::MENU_CLICK]);

		}

	}
	
}

void MainGame::gameLoop() {

	// init FPS limiter
	Pengine::FpsLimiter fpsLimiter;
	fpsLimiter.setTargetFPS(60.0f);

	
	// game loop while game state is play
	while (_gameState != GameState::EXIT) {

		// begin fps limiter
		fpsLimiter.begin();
		
		// update camera
		_camera.update();

		if (_gameState == GameState::PLAY || _gameState == GameState::GAMEOVER) {


			// update level, returns true if game over
			_gameOver = _level->updateBoxes(_score);

			// check if game is over
			gameOver();

			// check if level is complete
			if (_level->isLevelComplete()) {
				// increment level
				_currentLevel++;
				// delete level
				delete _level;
				// create the new level
				_level = new Level(_currentLevel,
									_screenWidth,
									_screenHeight,
									START_COLUMN,
									_score,
									_camera,
									_inputManager,
									_soundEffects[SoundEffects::BLOB_EXPLOSION],
									_soundEffects[SoundEffects::BOX_LANDING],
									_soundEffects[SoundEffects::LEVEL_COMPLETE]);
			}

		}

		// check if there is a pause request
		checkPause();

		// draw game
		drawGame();


		// update the menu
		updateMenu();

		// update input manager
		_inputManager.update();
		
		// process input
		processInput();

		// end fps limiter, receives current fps
		_fps = fpsLimiter.end();


	}

}

void MainGame::startNewGame() {
	// game is not over
	_gameOver = false;

	// reset score
	_score = 0;

	// create first level
	_currentLevel = 0;
	// create the new level
	_level = new Level(_currentLevel,
						_screenWidth,
						_screenHeight,
						START_COLUMN,
						_score,
						_camera,
						_inputManager,
						_soundEffects[SoundEffects::BLOB_EXPLOSION],
						_soundEffects[SoundEffects::BOX_LANDING],
						_soundEffects[SoundEffects::LEVEL_COMPLETE]);

}

void MainGame::gameOver() {
	// if game is over
	if (_gameOver) {
		// show game over menu

		// create the game over menu only once
		if (_gameState == GameState::PLAY) {
			// change game state to game over
			_gameState = GameState::GAMEOVER;
			// create the game over menu
			_menu = new MenuGameOver(_score,
									_screenWidth,
									_screenHeight,
									_camera,
									Pengine::ColorRGBA8(0, 255, 0, 255),
									Pengine::ColorRGBA8(255, 125, 0, 255),
									_soundEffects[SoundEffects::MENU_CLICK],
									_soundEffects[SoundEffects::GAMEOVER]);
		}
	}

}

void MainGame::checkPause() {
	// if escape or pause are clicked
	if (_inputManager.isKeyPressed(SDLK_ESCAPE) || _inputManager.isKeyPressed(SDLK_PAUSE)) {
		// pause game
		if (_gameState == GameState::PLAY) {
			// init the pause menu only once
			// play pause sound effect
			_soundEffects[SoundEffects::PAUSE].play(0, 7);
			// change game state to pause
			_gameState = GameState::PAUSE;
			// create the pause menu
			_menu = new MenuPause(_screenWidth,
									_screenHeight,
									_camera,
									Pengine::ColorRGBA8(0, 255, 0, 255),
									Pengine::ColorRGBA8(255, 125, 0, 255),
									_soundEffects[SoundEffects::MENU_CLICK]);
		}
	}

}

void MainGame::processInput() {
    SDL_Event evnt;

    // will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				// exit the game
				_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
				// capture mouse motion
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
				// capture 
				_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
				// release
				_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
                break;
        }
    
	}

}

void MainGame::drawGame() {
    // set the base depth to 1.0
    glClearDepth(1.0);
    // clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// bind shadder
	_textureProgram.use();

	// draw
	glActiveTexture(GL_TEXTURE0);

	// make sure shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// get projection matrix from the camera
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();

	// pass the projection matrix to the shader
	GLint pUniform = _textureProgram.getUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// draw the level
	if (_gameState != GameState::MAIN_MENU) {
		_level->draw();
	}

	// draw menu if there is one
	if (_menu != nullptr) {
		_menu->draw();
	}

	// unbind
	_textureProgram.unuse();

	// swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}


