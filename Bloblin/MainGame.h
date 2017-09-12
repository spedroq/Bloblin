#pragma once

#include <Pengine/PengineWindow.h>
#include <Pengine/GLSLProgram.h>
#include <Pengine/Camera2D.h>
#include <Pengine/InputManager.h>
#include <Pengine/AudioEngine.h>

#include "Level.h"
#include "MenuGameOver.h"
#include "MenuMain.h"
#include "MenuPause.h"

// game sate
enum class GameState {
	MAIN_MENU,
	PLAY,
	LEVEL_COMPLETE,
	GAMEOVER,
	PAUSE,
	EXIT
};

// soundeffect ids
const enum SoundEffects {
	MENU_CLICK,
	BOX_LANDING,
	BLOB_EXPLOSION,
	LEVEL_COMPLETE,
	GAMEOVER,
	PAUSE
};

// camera
const float CAMERA_SCALE = 1.0f;


class MainGame
{
public:
    MainGame();
    ~MainGame();

    // runs the game
    void run();

private:
	// the game window
	Pengine::Window _window;
	// the shader program
	Pengine::GLSLProgram _textureProgram;
	// handles input
	Pengine::InputManager _inputManager;
	// main camera
	Pengine::Camera2D _camera;
	// audio engine
	Pengine::AudioEngine _audioEngine;
	// holds the music
	Pengine::Music* _music;

	std::map<int, Pengine::SoundEffect> _soundEffects;

	// current game state
	GameState _gameState;

	// is game over
	bool _gameOver;

	// menu
	Menu* _menu;


	// vector of all levels
	Level* _level;

	// current level
	int _currentLevel;

	// window dimentions
	int _screenWidth, _screenHeight;

	// current fps
	float _fps;

	// game score
	int _score;


	// initializes the core systems
	void initSystems();
	// initializes the shaders
	void initShaders();
	// load all sound effects
	void loadSoundEffects();

	// main game loop for the program
	void gameLoop();
	// handles input processing
	void processInput();
	// renders the game
	void drawGame();

	// check if the game is over
	void gameOver();

	// check if the game has been paused
	void checkPause();

	// update the menu
	void updateMenu();

	// start a new game
	void startNewGame();
	
};

