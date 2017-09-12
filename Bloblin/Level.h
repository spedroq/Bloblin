#pragma once

#include <vector>

#include <Pengine/Camera2D.h>
#include <Pengine/SpriteBatch.h>
#include <Pengine/ResourceManager.h>
#include <Pengine/SpriteFont.h>
#include <Pengine/AudioEngine.h>

#include "Box.h"
#include "Blob.h"
#include "Bomb.h"
#include "Laser.h"
#include "DeathAnimation.h"
#include "PointsAnimation.h"

// set the types of blobs, 0 means the grid is empty
const enum BOX_COLOR {
	GREEN,
	LIGHT_BLUE,
	ORANGE,
	DARK_BLUE,
	RED,
	PINK,
	YELLOW
};
// keep a count of the types of blobs
const int NUM_BOX_COLORS = 7;

// frequency of spawning special boxes (spawn 1:SPECIALS_SPAWN_FREQUENCY)
const int SPECIALS_SPAWN_FREQUENCY = 1000;

// background offset
const int BACKGROUND_OFFSET_Y = -74;

// game shape
const int NUM_COLUMNS = 30;
const int NUM_ROWS = 20;
// game start spawn column
const int START_COLUMN = 15;
// rate of sliding right
const int SLIDE_RIGHT_RATE = 240;

// level score needed to pass level
const int SCORE_NEEDED = 250;

// hud coordinates
const glm::vec2 HUD_POS = glm::vec2(20, 595);

// animation
// breathing frequency
const int BREATHING_FREQUENCY = 10;

// level complete animation steps
const int LEVEL_COMPLETE_ANIMATION_STEPS = 60;

class Level
{
public:
	// load the level
	Level(int levelNum,
		int screenWidth,
		int screenHeight,
		int startNumColumns,
		int score,
		Pengine::Camera2D& camera,
		Pengine::InputManager& inputManager,
		Pengine::SoundEffect blobExplosionSE,
		Pengine::SoundEffect boxLandingSE,
		Pengine::SoundEffect levelCompleteSE);
	~Level();

	// update the boxes, returns true if game is over
	bool updateBoxes(int& score);

	// draw the level
	void draw();

	// accessors
	std::map<int, Box*> getBoxes() { return _boxes; }

	std::vector<Box*> getLevelData() { return _levelData; }

	std::vector<Box*> getBoxesOutOfBounds() { return _boxesOutofBounds; }
	
	Pengine::ColorRGBA8 getLastColorDestroyed() { return _lastColorDestroyed; }

	Pengine::ColorRGBA8 getLevelColor() { return _levelColor; }

	bool isLevelComplete();

	// mutators
	void pushLevelData(Box* data) { _levelData.push_back(data); }

private:
	// colour white
	Pengine::ColorRGBA8 _white;

	// sprite batch to handle background sprite
	Pengine::SpriteBatch _spriteBatch;
	// level complete sprite batch
	Pengine::SpriteBatch _levelCompleteSpriteBatch;
	// sprite batch to handle the nightmode flash
	Pengine::SpriteBatch _nightModeFlashSpriteBatch;
	// sprite batch to handle the sprite of the boxes
	Pengine::SpriteBatch _boxSpriteBatch;
	// sprite batch to handle the sprite of the death animations
	Pengine::SpriteBatch _deathSpriteBatch;
	// sprite batch to handle the sprite of the points animations
	Pengine::SpriteBatch _pointsSpriteBatch;
	// sprite batch for hud
	Pengine::SpriteBatch _hudSpriteBatch;
	// sprite font
	Pengine::SpriteFont* _spriteFont;

	// sound effect for blob explosion
	Pengine::SoundEffect _blobExplosionSoundEffect;
	// sound effect for the box landing
	Pengine::SoundEffect _boxLandingSoundEffect;
	// sound effect for level complete
	Pengine::SoundEffect _levelCompleteSoundEffect;

	// input manager
	Pengine::InputManager& _inputManager;
	// camera
	Pengine::Camera2D& _camera;

	// box colours
	std::map<int, Pengine::ColorRGBA8> _colors;
	// vector with the level data (for matrix index based parsing)
	std::vector<Box*> _levelData;
	// map of all boxes (for id based searching)
	std::map<int, Box*> _boxes;
	// vector of boxes out of bounds
	std::vector<Box*> _boxesOutofBounds;
	// vector of boxes to destroy
	std::vector<int> _boxesDestroyed;
	// vector of boxes to destroy
	std::vector<int> _boxesHighlighted;
	// vector of death animations
	std::vector<DeathAnimation*> _deathAnimations;
	// vector of points animations
	std::vector<PointsAnimation*> _pointsAnimations;

	// number of this level
	int _levelNum;
	// level colour
	Pengine::ColorRGBA8 _levelColor;

	// screen dimentions
	int _screenWidth;
	int _screenHeight;

	// new spawn ID for boxes
	int _newID;

	// current number of columns
	int _currentNumCols;
	// slide right counter
	int _slideRightCounter;

	// last colour to have been destroyed
	Pengine::ColorRGBA8 _lastColorDestroyed;

	// is true if game is over
	bool _gameOver;
	// is true if level has been completed
	bool _levelComplete;

	// true if nightmode is on
	bool _nightmode;

	// game score
	int _score;
	// score needed to pass the level
	int _scoreNeeded;
	// number of different colour types in the level
	int _numTypes;

	// level complete
	int _levelCompelteAnimationSteps;



	// init the level
	void initLevel();

	// returns a list of ids from the boxes above it
	std::vector<int> getBoxesInColumn(Box* box);

	// returns a list of ids from the boxes adjacent to it that share the same type
	std::vector<int> getAdjacentBoxes(Box* box);
	// returns a list of ids from the boxes adjacent to it that share the same type and have not been checked
	std::vector<int> getUncheckedAdjacentBoxes(Box* box);

	// spawn a box
	Box* Level::spawnBox(Box* box, int id, glm::vec2 position, int type);
	// spawn a blob
	Blob* spawnBlob(Blob* blob, int id, glm::vec2 position, int type);
	// spawn a bomb
	Bomb* spawnBomb(Bomb* bomb, int id, glm::vec2 position, int type);
	// spawn a laser
	Laser* spawnLaser(Laser* laser, int id, glm::vec2 position, int type);

	// spawn column
	void spawnColumn();

	// update the death animations
	void updateDeathAnimations();
	// update the points animations
	void Level::updatePointsAnimations();

	// highlight boxes hovered over
	void highlightBoxes();

	// destroy a box from the level data
	void destroyBox(int id);
	// checks the chain of boxes to destroy
	void checkDestroyBox(int id);
	// erases a specific box from the map
	void destroyBoxChain(int id);
	// destroys unchecked boxes
	void destroyUncheckedBox(int id);

	// destroy all boxes with the same colour as the bomb
	void destroyBoxesWithBomb(int boxColor);
	// destroy all boxes above the laser
	void destroyBoxesWithLaser(int id);

	// highlight boxes with a bomb explosion
	void highlightBoxesWithBomb(int boxColor);
	// highlight boxes affected by the laser
	void highlightBoxesWithLaser(int id);
	// check highlighted boxes
	void checkHighlightBox(int id);
	// check highlighted boxes that have not been checked yet
	void highlightUncheckedBox(int id);

	// activates the fall of the column above the box id received
	void activateBoxMovement(int id);
	// slide columns to fill an empty column
	void fillEmptyColumn(int emptyCol);
	// slide columns to the right
	void slideRight();
	// check if there is any column empty
	std::vector<int> checkEmptyColumns();

	// draw death animations
	void drawDeathAnimations();
	// draw points animations
	void drawPointsAnimations();
	// draw boxes
	void drawBoxes();
	// draw nightmode
	void drawNightmode();
	// renders the game hud
	void drawHud();
	// renders the level complete animation
	void drawLevelCompleteAnimation();

};

