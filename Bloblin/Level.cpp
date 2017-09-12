#include "Level.h"

#include "Blob.h"

#include <random>
#include <ctime>
#include <iostream>
#include <sstream>

Level::Level(int levelNum,
			int screenWidth,
			int screenHeight,
			int startNumColumns,
			int score,
			Pengine::Camera2D& camera,
			Pengine::InputManager& inputManager,
			Pengine::SoundEffect blobExplosionSE,
			Pengine::SoundEffect boxLandingSE,
			Pengine::SoundEffect levelCompleteSE) :
	_inputManager(inputManager),
	_camera(camera),
	_levelNum(levelNum),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_score(score),
	_levelColor(Pengine::ColorRGBA8(255, 255, 255, 255)),
	_lastColorDestroyed(Pengine::ColorRGBA8(255, 255, 255, 255)),
	_newID(0),
	_currentNumCols(startNumColumns),
	_slideRightCounter(SLIDE_RIGHT_RATE),
	_gameOver(false),
	_levelComplete(false),
	_numTypes(NUM_BOX_COLORS),
	_levelCompelteAnimationSteps(0)
{
	// set the sound effects

	// set the sound effect for blob explosion
	_blobExplosionSoundEffect = blobExplosionSE;
	// set the sound effect for the box landing
	_boxLandingSoundEffect = boxLandingSE;
	// set the sound effect for the level complete
	_levelCompleteSoundEffect = levelCompleteSE;
	
	// init the sprite batch for the boxes
	_boxSpriteBatch.init();

	// init level
	initLevel();

	// init the death animation sprite bacth
	_deathSpriteBatch.init();
	// init the points sprite batch
	_pointsSpriteBatch.init();
	// init the hud sprite bacth
	_hudSpriteBatch.init();
	// init sprite font
	_spriteFont = new Pengine::SpriteFont("../Fonts/Baloo-Regular.ttf", 64);
}

Level::~Level()
{
	//delete sprite font
	delete _spriteFont;

	// delete colors
	std::map<int, Pengine::ColorRGBA8>::iterator itrColors = _colors.begin();
	while (itrColors != _colors.end()) {
		// delete color and post incremente iterator
		itrColors = _colors.erase(itrColors++);
	}
	_colors.clear();

	// delete level
	for (int i = 0; i < _levelData.size(); i++) {
		// delete color and post incremente iterator
		delete _levelData[i];
	}
	_levelData.clear();

	// delete boxes
	std::map<int, Box*>::iterator itrBoxes = _boxes.begin();
	while (itrBoxes != _boxes.end()) {
		// delete box and post incremente iterator
		itrBoxes = _boxes.erase(itrBoxes++);
	}
	_boxes.clear();

	// delete boxes out of bounds
	for (int i = 0; i < _boxesOutofBounds.size(); i++) {
		delete _boxesOutofBounds[i];
	}
	_boxesOutofBounds.clear();

	// clear the vector of boxes destoryed
	_boxesDestroyed.clear();

	// clear the vector of highlighted boxes
	_boxesHighlighted.clear();

	// delete death animations
	for (int i = 0; i < _deathAnimations.size(); i++) {
		delete _deathAnimations[i];
	}
	_deathAnimations.clear();
	
	// delete points animations
	for (int i = 0; i < _pointsAnimations.size(); i++) {
		delete _pointsAnimations[i];
	}
	_pointsAnimations.clear();

}


void Level::initLevel() {
	// init the level
	_scoreNeeded = SCORE_NEEDED * (_levelNum+1) * (_levelNum + 1) + _score;

	// create the different colours and store them in _colors map
	_colors[BOX_COLOR::GREEN] = Pengine::ColorRGBA8(0, 255, 0, 255);
	_colors[BOX_COLOR::LIGHT_BLUE] = Pengine::ColorRGBA8(0, 255, 255, 255);
	_colors[BOX_COLOR::DARK_BLUE] = Pengine::ColorRGBA8(0, 125, 255, 255);
	_colors[BOX_COLOR::PINK] = Pengine::ColorRGBA8(255, 125, 200, 255);
	_colors[BOX_COLOR::RED] = Pengine::ColorRGBA8(255, 0, 0, 255);
	_colors[BOX_COLOR::ORANGE] = Pengine::ColorRGBA8(255, 125, 0, 255);
	_colors[BOX_COLOR::YELLOW] = Pengine::ColorRGBA8(255, 255, 0, 255);

	// initialize sprite batch
	_spriteBatch.init();
	// start sprite batch
	_spriteBatch.begin();

	// draw background
	// get destRect
	glm::vec4 destRect(0, BACKGROUND_OFFSET_Y, (float)_screenWidth / _camera.getScale(), (float)_screenHeight / _camera.getScale());
	// get uvRect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	// get white colour
	_white = Pengine::ColorRGBA8(255, 255, 255, 255);

	// check if it is time for nightmode
	if (_levelNum == 4 || _levelNum == 9 || _levelNum == 14 || _levelNum == 19) {
		// set nightmode true
		_nightmode = true;
		// draw night mode background
		_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/background/background_small_night.png").id, 0.0f, _white);
		// init the nightmode sprite batch used for the focus light
		_nightModeFlashSpriteBatch.init();
	}
	else {
		// if not time for nightmode then false
		_nightmode = false;
	}
	// set the level color
	if (_levelNum < 5) {
		// set to green
		_levelColor = _colors[BOX_COLOR::GREEN];
		// set the amount of different colours at every difficulty
		_numTypes = 4;
		// only draw background if there is no nightmode
		if (!_nightmode) {
			// draw the background
			_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/background/background_small_blue.png").id, 0.0f, _white);
		}
	}
	else if (_levelNum < 10) {
		// set to yellow
		_levelColor = _colors[BOX_COLOR::YELLOW];
		// set the amount of different colours at every difficulty
		_numTypes = 5;
		// only draw background if there is no nightmode
		if (!_nightmode) {
			// draw the background
			_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/background/background_small_yellow.png").id, 0.0f, _white);
		}
	}
	else if (_levelNum < 15) {
		// set to orange
		_levelColor = _colors[BOX_COLOR::ORANGE];
		// set the amount of different colours at every difficulty
		_numTypes = 6;
		// only draw background if there is no nightmode
		if (!_nightmode) {
			// draw the background
			_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/background/background_small_orange.png").id, 0.0f, _white);
		}
	}
	else {
		// set to red
		_levelColor = _colors[BOX_COLOR::RED];
		// set the amount of different colours at every difficulty
		_numTypes = 7;
		// only draw background if there is no nightmode
		if (!_nightmode) {
			// draw the background
			_spriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/background/background_small_red.png").id, 0.0f, _white);
		}
	}
	// init the score colour
	_lastColorDestroyed = _levelColor;

	// end sprite batch
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	// create gravity direction
	glm::vec2 direction;
	direction.x = 0;
	direction.y = -1;


	// create the random generator
	static std::mt19937 randomEngine(time(nullptr));
	// create the random generator of blob types
	std::uniform_int_distribution<int> randBlobType(0, _numTypes-1);

	// parse rows and add extra line
	for (int x = 0; x < NUM_ROWS; x++) {

		// parse columns
		for (int y = 0; y < NUM_COLUMNS; y++) {

			// get a random type of box
			int boxType = randBlobType(randomEngine);

			// get destRect
			glm::vec4 destRect(y * TILE_WIDTH, x * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);


			// positions are inverted
			glm::vec2 position;
			position.x = y * TILE_WIDTH;
			position.y = (x - 1) * TILE_WIDTH;

			Box* box = nullptr;

			// columns that are to be filled
			if (y < START_COLUMN) {
				// process the box
				box = spawnBox(box, _newID, position, boxType);
				// put the new box in the level data
				_levelData.push_back(box);

			}
			// empty columns
			else {
				// fill the level with empty space
				_levelData.push_back(nullptr);
			}

			// increment new ID
			_newID++;
		}
	}

}

void Level::spawnColumn() {

	// create the random generator
	static std::mt19937 randomEngine(time(nullptr));
	// create the random generator of blob types
	std::uniform_int_distribution<int> randBlobType(0, _numTypes-1);

	// parse rows in the first column
	int size = _levelData.size();
	int x = 0;
	for (int r = 0; r < size; r += NUM_COLUMNS) {

		// get a random type of box
		int boxType = randBlobType(randomEngine);

		// get destRect
		glm::vec4 destRect(0 * TILE_WIDTH, x * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

		// set the position
		glm::vec2 position;
		position.x = -1 * TILE_WIDTH;
		position.y = (x-1) * TILE_WIDTH;

		// create a new box to be filled by the spawner
		Box* box = nullptr;

		// process the box
		box = spawnBox(box, _newID, position, boxType);
		// increment id for future use
		_newID++;

		// place the blob in the grid
		_levelData[r] = box;

		// activate right slide movement
		_levelData[r]->setSlidingRight(true);
		_levelData[r]->setSlideRightSteps(SLIDE_RIGHT_STEPS_PER_TILE);

		// increment x coord
		x++;
	}

}

Box* Level::spawnBox(Box* box, int id, glm::vec2 position, int type) {

	// calculate the frequency of specials to be spawned
	int specialsFrequency = SPECIALS_SPAWN_FREQUENCY / ((_levelNum + 2) / 2 );

	// create the random generator
	static std::mt19937 randomEngine(time(nullptr));
	// create the random generator of blob spawn
	std::uniform_int_distribution<int> randBoxType(0, specialsFrequency);

	// create the random generator of specials spawn
	static std::uniform_int_distribution<int> randSpecialType(1, NUM_TYPES_BOXES - 1);

	// get a random value
	int randomType = randBoxType(randomEngine);
	if (randomType < specialsFrequency) {
		// create a new blob
		Blob* blob = nullptr;
		box = spawnBlob(blob, id, position, type);
	}
	else if (randomType == specialsFrequency) {
		// get a random special type
		int randomSpecial = randSpecialType(randomEngine);
		// spawn bombs
		if(randomSpecial == BOX_TYPE::BOMB_TYPE){
			// create a new bomb
			Bomb* bomb = nullptr;
			box = spawnBomb(bomb, id, position, type);
		}
		// spawn lasers
		else if (randomSpecial == BOX_TYPE::LASER_TYPE) {
			// create a new laser
			Laser* laser = nullptr;
			box = spawnLaser(laser, id, position, type);
		}
		
	}
	// return the spawned box
	return box;
}

Blob* Level::spawnBlob(Blob* blob, int id, glm::vec2 position, int type) {
	// create a new blob
	blob = new Blob(id, position, _colors[type], type, _boxLandingSoundEffect);
	// add it to the map of boxes
	_boxes[id] = blob;
	// return it
	return blob;
}

Bomb* Level::spawnBomb(Bomb* bomb, int id, glm::vec2 position, int type) {
	// create a new bomb
	bomb = new Bomb(id, position, _colors[type], type, _boxLandingSoundEffect);
	// add it to the map of boxes
	_boxes[id] = bomb;
	// return it
	return bomb;
}

Laser* Level::spawnLaser(Laser* laser, int id, glm::vec2 position, int type) {
	// create a new laser
	laser = new Laser(id, position, _colors[type], type, _boxLandingSoundEffect);
	// add it to the map of boxes
	_boxes[id] = laser;
	// return it
	return laser;
}

bool Level::updateBoxes(int& score) {

	// update death animations
	updateDeathAnimations();

	// update all the points animations
	updatePointsAnimations();

	// clear the list of boxes to be destroyed
	_boxesDestroyed.clear();

	// update each box

	// parse all boxes to update each individual box
	for (int i = 0; i < _levelData.size(); i++) {
		if (_levelData[i] != nullptr) {
			// update returns true if there was a click on the blob
			if (_levelData[i]->update(_inputManager, &_camera)) {

				// check if the box is to be destroyed
				checkDestroyBox(_levelData[i]->getID());

				// parse boxes to be destroyed
				if (_boxesDestroyed.size() > 0) {
					// check for bombs being destroyed
					// flag to see if there was a bomb in the chain
					bool bombInChain = false;
					// check if the box clicked is a bomb
					if (_levelData[i]->getBoxType() == BOX_TYPE::BOMB_TYPE) {
						bombInChain = true;
					}
					for (int b = 0; b < _boxesDestroyed.size(); b++) {
						if (_boxes[_boxesDestroyed[b]] != nullptr) {
							// check if the box to be destroyed is a bomb
							if (_boxes[_boxesDestroyed[b]]->getBoxType() == BOX_TYPE::BOMB_TYPE) {
								// there is a bomb in the chain
								bombInChain = true;
							}
						}
					}

					// if there is a bomb in the chain destroy all of the same color
					if (bombInChain) {
						destroyBoxesWithBomb(_levelData[i]->getType());
					}

					// check for laser's being destroyed
					// check if the box clicked is a laser
					if (_levelData[i]->getBoxType() == BOX_TYPE::LASER_TYPE) {
						destroyBoxesWithLaser(_levelData[i]->getID());
					}
					for (int b = 0; b < _boxesDestroyed.size(); b++) {
						if (_boxes[_boxesDestroyed[b]] != nullptr) {
							// check if the box to be destroyed is a laser
							if (_boxes[_boxesDestroyed[b]]->getBoxType() == BOX_TYPE::LASER_TYPE) {
								// there is a laser in the chain
								destroyBoxesWithLaser(_boxes[_boxesDestroyed[b]]->getID());
							}
						}
					}
				}

				int size = _boxesDestroyed.size();
				int combo = 0;
				for (int b = 0; b < size; b++) {
					if (_boxes[_boxesDestroyed[b]] != nullptr) {
						combo++;
					}
				}

				// increment score only if game is not over
				if (!_gameOver) {
					// score gained due to click
					int points = (combo * combo);
					// increment score
					score += points;
					if (combo != 0) {

						// get the mouse coordinates
						glm::vec2 mouseCoords = _inputManager.getMouseCoords();
						// convert screen coordinates to world coordinates to make the click
						mouseCoords = _camera.convertScreenToWorld(mouseCoords);

						// get the string version of the points gained
						std::ostringstream oss;
						oss << "+" << points;
						std::string pointsString = oss.str();

						// offset the spawn of the animation so the next one does not overlap it
						mouseCoords.y += 25;
						// create points animations if score increased
						_pointsAnimations.push_back(new PointsAnimation(mouseCoords, pointsString, _levelData[i]->getColor()));

						// check for mega combos
						if (size >= 5) {
							// get the string version of the combo
							std::ostringstream oss;
							oss << "x" << combo << " Combo";
							std::string comboString = oss.str();
							// offset the spawn of the animation so as to not overlap the previous one
							mouseCoords.y -= 50;
							// create points animations if score increased
							_pointsAnimations.push_back(new PointsAnimation(mouseCoords, comboString, _levelData[i]->getColor()));

						}

					}
				}

				// parse the list of boxes to be destroyed
				size = _boxesDestroyed.size();
				for (int b = 0; b < size; b++) {
					
					if (_boxes[_boxesDestroyed[b]] != nullptr) {
						// create death animation
						_deathAnimations.push_back(new DeathAnimation(_boxes[_boxesDestroyed[b]]->getColor(), _boxes[_boxesDestroyed[b]]->getPosition()));

						// activate the movement of the boxes related to the ones destroyed
						activateBoxMovement(_boxesDestroyed[b]);
						// update the boxes in the level (update level matrix)
						destroyBox(_boxesDestroyed[b]);
						// update the boxes in the list of boxes
						destroyBoxChain(_boxesDestroyed[b]);
					}
				}

				



			}
		}
	}
	// set all boxes back to unchecked
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		if (_levelData[i] != nullptr) {
			_levelData[i]->setChecked(false);
		}
	}

	// check if any column is now empty
	std::vector<int> emptyCols = checkEmptyColumns();
	size = emptyCols.size();

	// parse back to front so that as column get pushed left the column number of the ones behind them are not affected
	for (int i = size - 1; i >= 0; i--) {

		// fill the empty column by slide everything left
		fillEmptyColumn(emptyCols[i]);
		// reduce the number of columns
		_currentNumCols--;
	}

	// update boxes out of bounds (game over boxes fallin animation)
	for (int i = 0; i < _boxesOutofBounds.size();) {
		// update boxes that are out of bounds
		_boxesOutofBounds[i]->update(_inputManager, &_camera);
		int animationSteps = _boxesOutofBounds[i]->outOfBoundsFall();
		if (animationSteps == 0) {
			// delete box out of bounds if animations is complete
			delete _boxesOutofBounds[i];
			_boxesOutofBounds[i] = _boxesOutofBounds.back();
			_boxesOutofBounds.pop_back();
		}
		else {
			// only increment the index if no item was deleted
			i++;
		}
	}

	// slide to the right
	slideRight();

	// randomly animate boxes to breathe
	// create the random generator
	static std::mt19937 randomEngine(time(nullptr));
	// create the random generator of blob types
	static std::uniform_int_distribution<int> randBlobIndex(0, _levelData.size() - 1);
	// create the random generator of number of blobs to set breathing
	static std::uniform_int_distribution<int> randBlobBreathingNum(0, BREATHING_FREQUENCY);

	// create a random number of breathing animations
	int randomBreathingNum = randBlobBreathingNum(randomEngine);
	if (randomBreathingNum == 0) {
		// create a random index to apply the breathing animation
		int index = randBlobIndex(randomEngine);
		// check if the random grid member is not empty
		if (_levelData[index] != nullptr) {
			// only do breathing animation if the box is not highlighted
			if (!_levelData[index]->getHighlighted()) {
				// set the breathing animation
				_levelData[index]->setBreathing(BREATHING_STEPS);
				_levelData[index]->breatheAnimation();
			}
		}
	}
	// update the score
	_score = score;
	// check if level has been completed for the first time
	if (_score >= _scoreNeeded && _levelComplete == false) {

		// init the level complete sprite batch
		_levelCompleteSpriteBatch.init();
		// begin the sprite batch
		_levelCompleteSpriteBatch.begin();

		// set level complete
		_levelComplete = true;

		// start level complete animation
		_levelCompelteAnimationSteps = LEVEL_COMPLETE_ANIMATION_STEPS;

		// draw the level complete

		// draw level complete animation
		// get destRect
		glm::vec4 destRect(0, BACKGROUND_OFFSET_Y, (float)_screenWidth / _camera.getScale(), (float)_screenHeight / _camera.getScale());
		// get uvRect
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		// draw the level complete animation
		_levelCompleteSpriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/level_complete.png").id, 1.0f, _levelColor);

		// end sprite batch
		_levelCompleteSpriteBatch.end();
		// render the batch
		_levelCompleteSpriteBatch.renderBatch();


		// level complete sound effect (same channel as clicks)
		_levelCompleteSoundEffect.play(0, 6);

		// reduce the number of steps in the level complete animation
		_levelCompelteAnimationSteps--;
	}
	// check if level has been previously completed
	else if (_levelComplete) {
		// reduce the number of steps in the level complete animation
		_levelCompelteAnimationSteps--;
	}

	// highlight the boxes
	highlightBoxes();

	// return whether game is over or not
	return _gameOver;
}

void Level::updatePointsAnimations() {
	// parse all the points aniamtions
	for (int i = 0; i < _pointsAnimations.size();) {
		// update the points hanimation
		// if it is finished it returns true
		if (_pointsAnimations[i]->update()) {
			// delete the points animation
			delete _pointsAnimations[i];
			_pointsAnimations[i] = _pointsAnimations.back();
			_pointsAnimations.pop_back();
		}
		else {
			// only increment the index if no item was deleted
			i++;
		}
	}
}

void Level::updateDeathAnimations() {
	// parse all the death aniamtions
	for (int i = 0; i < _deathAnimations.size();){
		// update the death hanimation
		// if it is finished it returns true
		if (_deathAnimations[i]->update()) {
			// delete the death animation
			delete _deathAnimations[i];
			_deathAnimations[i] = _deathAnimations.back();
			_deathAnimations.pop_back();
		}
		else {
			// only increment the index if no item was deleted
			i++;
		}
	}

}

void Level::highlightBoxes() {

	// set all boxes back to un highlighted
	int size = _boxesHighlighted.size();
	for (int i = 0; i < size; i++) {
		if (_boxes[_boxesHighlighted[i]] != nullptr) {
			// unhighlight the box
			_boxes[_boxesHighlighted[i]]->setHighlighted(false);
			// reset the texture
			_boxes[_boxesHighlighted[i]]->resetTextureID();
		}
	}

	// clear the list of boxes to be highlighted
	_boxesHighlighted.clear();

	// parse all boxes to update each individual box
	for (int i = 0; i < _levelData.size(); i++) {
		if (_levelData[i] != nullptr) {
			// update returns true if there was a click on the blob
			if (_levelData[i]->checkHover(_inputManager, &_camera)) {

				// check if the box is to be highlighted
				checkHighlightBox(_levelData[i]->getID());

				if (_boxesHighlighted.size() > 0) {
					// check for bombs being highlighted
					// flag to see if there was a bomb in the chain
					bool bombInChain = false;
					// check if the box clicked is a bomb
					if (_levelData[i]->getBoxType() == BOX_TYPE::BOMB_TYPE) {
						bombInChain = true;
					}
					for (int b = 0; b < _boxesHighlighted.size(); b++) {

						if (_boxes[_boxesHighlighted[b]] != nullptr) {
							// check if the box to be highlighted is a bomb
							if (_boxes[_boxesHighlighted[b]]->getBoxType() == BOX_TYPE::BOMB_TYPE) {
								// there is a bomb in the chain
								bombInChain = true;
							}
						}
					}

					// if there is a bomb in the chain destroy all of the same color
					if (bombInChain) {
						highlightBoxesWithBomb(_levelData[i]->getType());
					}


					// check for laser's being highlighted
					// check if the box clicked is a laser
					if (_levelData[i]->getBoxType() == BOX_TYPE::LASER_TYPE) {
						highlightBoxesWithLaser(_levelData[i]->getID());
					}
					for (int b = 0; b < _boxesHighlighted.size(); b++) {

						if (_boxes[_boxesHighlighted[b]] != nullptr) {
							// check if the box to be highlighted is a laser
							if (_boxes[_boxesHighlighted[b]]->getBoxType() == BOX_TYPE::LASER_TYPE) {
								// there is a laser in the chain
								highlightBoxesWithLaser(_boxes[_boxesHighlighted[b]]->getID());
							}
						}
					}
				}

				// parse the list of boxes to be highlighted
				int size = _boxesHighlighted.size();
				for (int b = 0; b < size; b++) {
					// highlight all the boxes
					_boxes[_boxesHighlighted[b]]->highlight();
				}
			}
		}
	}
	// set all boxes back to unchecked
	size = _levelData.size();
	for (int i = 0; i < size; i++) {
		if (_levelData[i] != nullptr) {
			// uncheck the box
			_levelData[i]->setChecked(false);
		}
	}

}

void Level::destroyBoxesWithBomb(int boxColor) {
	// clear the list of boxes destroyed
	_boxesDestroyed.clear();
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// if not nullptr
		if (_levelData[i] != nullptr) {
			// get the box color and compare to see if it is the same type
			if(_levelData[i]->getType() == boxColor){
				// if so add to the list to be destroyed
				_boxesDestroyed.push_back(_levelData[i]->getID());
			}
		}
	}

}

void Level::highlightBoxesWithBomb(int boxColor) {
	// clear the boxes to be destroyed
	_boxesHighlighted.clear();
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// if not nullptr
		if (_levelData[i] != nullptr) {
			// get the box color and compare to see if it is the same type
			if (_levelData[i]->getType() == boxColor) {
				// if so add to the list to be destroyed
				_boxesHighlighted.push_back(_levelData[i]->getID());
			}
		}
	}

}

void Level::destroyBoxesWithLaser(int id) {
	
	// find the boxes above the current box
	std::vector<int> boxesAbove = getBoxesInColumn(_boxes[id]);

	// add them to the boxes that were destroyed
	int size = boxesAbove.size();
	for (int i = 0; i < size; i++) {
		_boxesDestroyed.push_back(boxesAbove[i]);
	}

}

void Level::highlightBoxesWithLaser(int id) {

	// find the boxes above the current box
	std::vector<int> boxesAbove = getBoxesInColumn(_boxes[id]);

	// add them to the boxes that were destroyed
	int size = boxesAbove.size();
	for (int i = 0; i < size; i++) {
		_boxesHighlighted.push_back(boxesAbove[i]);
	}

}

void Level::destroyBoxChain(int id) {

	// delete the box
	_boxes.erase(id);

}

void Level::checkDestroyBox(int id) {

	bool destroy = false;

	std::vector<int> adjacentBoxes;

	// check the box
	_boxes[id]->setChecked(true);

	// get adjacent boxes that share the same type
	adjacentBoxes = getAdjacentBoxes(_boxes[id]);

	// destroy box ONLY if it has neighbours of the same type!
	int adjacentBoxesSize = adjacentBoxes.size();
	if (adjacentBoxesSize > 0) {
		// destroy self
		destroy = true;
		// destroy neighbours of the same type
		for (int b = 0; b < _levelData.size(); b++) {
			for (int a = 0; a < adjacentBoxesSize; a++) {
				if (_levelData[b] != nullptr) {
					if (_levelData[b]->getID() == adjacentBoxes[a]) {
						// destroy boxes
						destroyUncheckedBox(_levelData[b]->getID());
						// skip other adjacent boxes
						a = adjacentBoxesSize;
					}
				}
			}
		}
	}

	// destroy box
	if (destroy == true) {
		// play the sound effect of the blob being destroyed
		_blobExplosionSoundEffect.play(0, 0);
		// add the box to be destroyed
		_boxesDestroyed.push_back(id);
		// get the color that was destroyed only if game has not ended
		if (_gameOver == 0) {
			_lastColorDestroyed = _boxes[id]->getColor();
		}
		
	}
}

void Level::checkHighlightBox(int id) {

	bool destroy = false;

	std::vector<int> adjacentBoxes;

	// check the box
	_boxes[id]->setChecked(true);

	// get adjacent boxes that share the same type
	adjacentBoxes = getAdjacentBoxes(_boxes[id]);

	// destroy box ONLY if it has neighbours of the same type!
	int adjacentBoxesSize = adjacentBoxes.size();
	if (adjacentBoxesSize > 0) {
		// destroy self
		destroy = true;
		// destroy neighbours of the same type
		for (int b = 0; b < _levelData.size(); b++) {
			for (int a = 0; a < adjacentBoxesSize; a++) {
				if (_levelData[b] != nullptr) {
					if (_levelData[b]->getID() == adjacentBoxes[a]) {
						// destroy boxes
						highlightUncheckedBox(_levelData[b]->getID());
						// skip other adjacent boxes
						a = adjacentBoxesSize;
					}
				}
			}
		}
	}

	// destroy box
	if (destroy == true) {
		// add the box to be destroyed
		_boxesHighlighted.push_back(id);
		// get the color that was destroyed only if game has not ended
		if (_gameOver == 0) {
			_lastColorDestroyed = _boxes[id]->getColor();
		}

	}
}

void Level::destroyUncheckedBox(int id) {

	// check the box
	_boxes[id]->setChecked(true);

	// get adjacent boxes that share the same type, that have not been checked yet
	std::vector<int> adjacentBoxes = getUncheckedAdjacentBoxes(_boxes[id]);
	
	int adjacentBoxesSize = adjacentBoxes.size();
	if (adjacentBoxesSize > 0) {
		// destroy neighbours of the same type
		for (int b = 0; b < _levelData.size(); b++) {
			for (int a = 0; a < adjacentBoxesSize; a++) {
				if (_levelData[b] != nullptr) {
					if (_levelData[b]->getID() == adjacentBoxes[a]) {
						// destroy the neighbour's neighbours of the same type
						destroyUncheckedBox(_levelData[b]->getID());
						// skip other adjacent boxes
						a = adjacentBoxesSize;
					}
				}
			}
		}
	}

	// add the box to the list to be deleted
	_boxesDestroyed.push_back(id);
}

void Level::highlightUncheckedBox(int id) {

	// check the box
	_boxes[id]->setChecked(true);

	// get adjacent boxes that share the same type, that have not been checked yet
	std::vector<int> adjacentBoxes = getUncheckedAdjacentBoxes(_boxes[id]);

	int adjacentBoxesSize = adjacentBoxes.size();
	if (adjacentBoxesSize > 0) {
		// destroy neighbours of the same type
		for (int b = 0; b < _levelData.size(); b++) {
			for (int a = 0; a < adjacentBoxesSize; a++) {
				if (_levelData[b] != nullptr) {
					if (_levelData[b]->getID() == adjacentBoxes[a]) {
						// destroy the neighbour's neighbours of the same type
						highlightUncheckedBox(_levelData[b]->getID());
						// skip other adjacent boxes
						a = adjacentBoxesSize;
					}
				}
			}
		}
	}

	// add the box to the list to be deleted
	_boxesHighlighted.push_back(id);
}

void Level::activateBoxMovement(int id) {
	if (_boxes[id] != nullptr) {
		// get the position
		glm::vec2 position = _boxes[id]->getPosition();

		// activate motion only on those above the box
		// find the boxes above the current box
		std::vector<int> boxesAbove = getBoxesInColumn(_boxes[id]);
		int size = boxesAbove.size();
		if (size > 0) {
			for (int b = 0; b < size; b++) {
				// set the box to falling
				_boxes[boxesAbove[b]]->setFalling(true);
				_boxes[boxesAbove[b]]->addFallSteps(FALL_STEPS_PER_TILE);
			}
		}
	}

}

std::vector<int> Level::getBoxesInColumn(Box* box) {
	// prep boxesInColumn to return a list of boxes in the column above the box that was received
	std::vector<int> boxesInColumn;

	// get the box id
	int boxID = box->getID();
	// parse boxes
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// check id until we find the box that was sent to us
		if (_levelData[i] != nullptr) {
			if (_levelData[i]->getID() == boxID) {
				// parse the cols above
				for (int b = i + NUM_COLUMNS; b < size; b += NUM_COLUMNS) {
					if (_levelData[b] != nullptr) {
						// add the boxes in the column above
						boxesInColumn.push_back(_levelData[b]->getID());
					}
				}
				// skip the rest of the boxes
				i = size;
			}
		}
	}
	// return the boxes in the collumn above
	return boxesInColumn;

}

std::vector<int> Level::getAdjacentBoxes(Box* box) {

	// prep adjacentBoxes to return a list of boxes adjacent to it
	std::vector<int> adjacentBoxes;

	// get the box id
	int boxID = box->getID();
	// parse boxes
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// check id until we find the box that was sent to us
		if (_levelData[i] != nullptr) {
			if (_levelData[i]->getID() == boxID) {

				int boxType = _levelData[i]->getType();

				// box to the left
				int leftIndex = i - 1;
				// check if the box to the left is not on the last column (in which would mean we are checking the first column)
				if (leftIndex%NUM_COLUMNS != NUM_COLUMNS-1) {
					if (leftIndex >= 0 && _levelData[leftIndex] != nullptr) {
						// check the type of the box
						int leftBoxType = _levelData[leftIndex]->getType();
						if (leftBoxType == boxType) {
							// add adjacent box
							adjacentBoxes.push_back(_levelData[leftIndex]->getID());
						}
					}
				}				

				// box to the right
				int rightIndex = i + 1;
				// check if the box to the right is not on the first column (in which case we are checking the last column)
				if (rightIndex%NUM_COLUMNS != 0) {
					if (rightIndex < size && _levelData[rightIndex] != nullptr) {
						// check the type of the box
						int rightBoxType = _levelData[rightIndex]->getType();
						if (rightBoxType == boxType) {
							// add adjacent box
							adjacentBoxes.push_back(_levelData[rightIndex]->getID());
						}
					}
				}

				// box above
				int topIndex = i + NUM_COLUMNS;
				if (topIndex < size && _levelData[topIndex] != nullptr) {
					// check the type of the box
					int topBoxType = _levelData[topIndex]->getType();
					if (topBoxType == boxType) {
						// add adjacent box
						adjacentBoxes.push_back(_levelData[topIndex]->getID());
					}
				}

				// box below
				int botIndex = i - NUM_COLUMNS;
				if (botIndex >= 0 && _levelData[botIndex] != nullptr) {
					// check the type of the box
					int botBoxType = _levelData[botIndex]->getType();
					if (botBoxType == boxType) {
						// add adjacent box
						adjacentBoxes.push_back(_levelData[botIndex]->getID());
					}
				}

				// skip the rest of the boxes
				i = size;
			}
		}

	}
	// return the list of adjacent boxes
	return adjacentBoxes;

}


std::vector<int> Level::getUncheckedAdjacentBoxes(Box* box) {
	// prep adjacentBoxes to return a list of boxes adjacent to it
	std::vector<int> adjacentBoxes;

	// get the box id
	int boxID = box->getID();
	// parse boxes
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// check id until we find the box that was sent to us
		if (_levelData[i] != nullptr) {
			if (_levelData[i]->getID() == boxID) {

				// get the box type
				int boxType = _levelData[i]->getType();

				// box to the left
				int leftIndex = i - 1;
				// check if the box to the left is not on the last column (in which would mean we are checking the first column)
				if (leftIndex%NUM_COLUMNS != NUM_COLUMNS - 1) {
					if (leftIndex >= 0 && _levelData[leftIndex] != nullptr) {
						// check the type of the box and check if it has been checked before
						int leftBoxType = _levelData[leftIndex]->getType();
						if (leftBoxType == boxType && _levelData[leftIndex]->getChecked() == false) {
							// add adjacent box
							adjacentBoxes.push_back(_levelData[leftIndex]->getID());
						}
					}
				}

				// box to the right
				int rightIndex = i + 1;
				// check if the box to the right is not on the first column (in which case we are checking the last column)
				if (rightIndex%NUM_COLUMNS != 0) {
					if (rightIndex < size && _levelData[rightIndex] != nullptr) {
						// check the type of the box and check if it has been checked before
						int rightBoxType = _levelData[rightIndex]->getType();
						if (rightBoxType == boxType && _levelData[rightIndex]->getChecked() == false) {
							// add adjacent box
							adjacentBoxes.push_back(_levelData[rightIndex]->getID());
						}
					}
				}

				// box above
				int topIndex = i + NUM_COLUMNS;
				if (topIndex < size && _levelData[topIndex] != nullptr) {
					// check the type of the box and check if it has been checked before
					int topBoxType = _levelData[topIndex]->getType();
					if (topBoxType == boxType && _levelData[topIndex]->getChecked() == false) {
						// add adjacent box
						adjacentBoxes.push_back(_levelData[topIndex]->getID());
					}
				}

				// box below
				int botIndex = i - NUM_COLUMNS;
				if (botIndex >= 0 && _levelData[botIndex] != nullptr) {
					// check the type of the box and check if it has been checked before
					int botBoxType = _levelData[botIndex]->getType();
					if (botBoxType == boxType && _levelData[botIndex]->getChecked() == false) {
						// add adjacent box
						adjacentBoxes.push_back(_levelData[botIndex]->getID());
					}
				}

				// skip the rest of the boxes
				i = size;
			}
		}

	}
	// return adjacent unchecked boxes
	return adjacentBoxes;

}

void Level::destroyBox(int id) {

	// parse boxes
	int size = _levelData.size();
	for (int i = 0; i < size; i++) {
		// if this is the box we are looking for and not nullptr
		if (_levelData[i] != nullptr && _levelData[i]->getID() == id) {
			// parse all the boxes above it
			for (int b = i; b < size; b += NUM_COLUMNS) {
				// check if it is the last row
				int boxAbove = (b + NUM_COLUMNS);
				if (boxAbove >= size) {
					_levelData[b] = nullptr;
				}
				// otherwise move them down
				else {
					_levelData[b] = _levelData[boxAbove];
				}
			}
			// skip other boxes
			i = size;
		}
	}
}

void Level::fillEmptyColumn(int emptyCol) {

	// get the size of the level data
	int size = _levelData.size();

	// parse columns in the first row
	for (int i = 0; i < size; i++) {

		
		// check if current column is beyond the empty column
		int currentCol = i%NUM_COLUMNS;
		if (currentCol >= emptyCol) {
			// if this is the last column fill with nullptr
			if (currentCol == (NUM_COLUMNS - 1)) {
				_levelData[i] = nullptr;
			}
			else {
				int rightIndex = i + 1;
				// get the box to the right
				_levelData[i] = _levelData[rightIndex];

				// check if the new box is empty
				if (_levelData[i] != nullptr) {
					// do the left slide
					_levelData[i]->setSlidingLeft(true);
					_levelData[i]->addSlideLeftSteps(SLIDE_LEFT_STEPS_PER_TILE);
				}
			}
			
		}
	
	}
}

std::vector<int> Level::checkEmptyColumns() {

	// create the list of empty columns
	std::vector<int> emptyCols;

	// get the size of the level data
	int size = _levelData.size();

	// parse columns in the first row
	for (int i = 0; i < _currentNumCols; i++) {
		bool emptyCol = true;
		// parse rows of the column
		for (int b = i; b < size; b += NUM_COLUMNS) {
			// check if the column is not empty
			if (_levelData[b] != nullptr) {
				// if there is a non nullptr it is not empty
				emptyCol = false;
			}
		}
		// if the column is empty add it to the list
		if (emptyCol) {
			emptyCols.push_back(i);
		}
	}	

	return emptyCols;
}

void Level::slideRight() {

	if (!_levelComplete) {
		// check if it is time to slide to the right
		if (_slideRightCounter == 0) {
			// check losing condition
			if ((_currentNumCols + 1) <= NUM_COLUMNS) {
				// parse the level data
				int size = _levelData.size();
				for (int c = NUM_COLUMNS - 1; c >= 0; c--) {
					// parse the rows of each column
					for (int i = c; i < size; i += NUM_COLUMNS) {

						// activate movement right
						if (_levelData[i] != nullptr) {
							_levelData[i]->setSlidingRight(true);
							_levelData[i]->setSlideRightSteps(SLIDE_RIGHT_STEPS_PER_TILE);
						}

						// update grid
						// set new column to nullptr
						if (i%NUM_COLUMNS == 0) {
							_levelData[i] = nullptr;
						}
						// copy box to the left
						else {
							_levelData[i] = _levelData[i - 1];
						}


					}
				}
				// spawn new column
				spawnColumn();
				_currentNumCols++;
			}
			// game over
			else {
				// GAME OVER
				// do the game over animation

				// parse the level data
				int size = _levelData.size();
				for (int c = NUM_COLUMNS - 1; c >= 0; c--) {
					// parse the rows of each column
					for (int i = c; i < size; i += NUM_COLUMNS) {

						// activate movement right
						if (_levelData[i] != nullptr) {
							_levelData[i]->setSlidingRight(true);
							_levelData[i]->setSlideRightSteps(SLIDE_RIGHT_STEPS_PER_TILE);
						}

						// update grid
						// set new column to nullptr
						if (i%NUM_COLUMNS == 0) {
							_levelData[i] = nullptr;
						}
						// copy box to the left
						else {
							// if there is boxes out of bounds add them to the out of bounds vector
							if (i%NUM_COLUMNS == NUM_COLUMNS - 1) {
								if (_levelData[i] != nullptr) {
									// 
									Box* box = _levelData[i];
									_boxesOutofBounds.push_back(box);
									box->setOutOfBoundsAnimationSteps(180);
								}
							}
							// take the box to the left of it
							_levelData[i] = _levelData[i - 1];
						}


					}
				}
				// spawn new column
				spawnColumn();
				// increment current column
				_currentNumCols++;
				// start the slide right counter
				_slideRightCounter = SLIDE_RIGHT_RATE;
				// set game over to true
				_gameOver = true;
			}
			// reset slide counter
			_slideRightCounter = SLIDE_RIGHT_RATE;
		}
		else {
			// decrease slide counter
			_slideRightCounter--;
		}

	}

}

bool Level::isLevelComplete() {
	// check if level is complete and if level complete animation has ended
	if (_levelComplete && _levelCompelteAnimationSteps <= 0) {
		return true;
	}
	else {
		return false;
	}
}

void Level::draw() {

	// render the batch to draw the level
	_spriteBatch.renderBatch();

	// draw the death animations
	drawDeathAnimations();

	// draw the boxes
	drawBoxes();

	// draw the nightmode focus light
	drawNightmode();

	// draw the points animations
	drawPointsAnimations();

	// render the heads up display
	drawHud();

	// draw level complete animation if level is complete
	drawLevelCompleteAnimation();
}

void Level::drawDeathAnimations() {
	// start sprite batch
	_deathSpriteBatch.begin();
	// draw all boxes to the spritebatch
	int numDeaths = _deathAnimations.size();
	for (int i = 0; i < numDeaths; i++) {
		_deathAnimations[i]->draw(_deathSpriteBatch);
	}

	// end spritebatch and render it
	_deathSpriteBatch.end();
	// render the batch
	_deathSpriteBatch.renderBatch();
}

void Level::drawPointsAnimations() {
	// start sprite batch
	_pointsSpriteBatch.begin();
	// draw all boxes to the spritebatch
	int numDeaths = _pointsAnimations.size();
	for (int i = 0; i < numDeaths; i++) {
		_pointsAnimations[i]->draw(_pointsSpriteBatch);
	}

	// end spritebatch and render it
	_pointsSpriteBatch.end();
	// render the bacth
	_pointsSpriteBatch.renderBatch();
}

void Level::drawBoxes() {
	// start sprite batch
	_boxSpriteBatch.begin();
	// draw all boxes to the spritebatch
	int numBoxes = _levelData.size();
	for (int i = 0; i < numBoxes; i++) {
		if (_levelData[i] != nullptr) {
			_levelData[i]->draw(_boxSpriteBatch);
		}
	}
	// draw all the out of bounds boxes
	numBoxes = _boxesOutofBounds.size();
	for (int i = 0; i < numBoxes; i++) {
		if (_boxesOutofBounds[i] != nullptr) {
			_boxesOutofBounds[i]->draw(_boxSpriteBatch);
		}
	}

	// end spritebatch and render it
	_boxSpriteBatch.end();
	// render the bacth
	_boxSpriteBatch.renderBatch();	
}

void Level::drawNightmode() {
	// check if nightmode is on
	if (_nightmode) {

		// being the sprite batch
		_nightModeFlashSpriteBatch.begin();

		// get the mouse coordinates
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		// convert screen coordinates to world coordinates to make the click
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);

		// add the light focus for the night mode
		// set the uvRect
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		// set the destRect
		glm::vec4 destRect;
		destRect.x = mouseCoords.x - _screenWidth;
		destRect.y = (mouseCoords.y - _screenHeight);
		destRect.z = _screenWidth * 2;
		destRect.w = _screenHeight * 2;

		// draw the box
		_nightModeFlashSpriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/night_focus.png").id, 1.0f, _white);

		// end the sprite bacth
		_nightModeFlashSpriteBatch.end();
		// render the spritebatch
		_nightModeFlashSpriteBatch.renderBatch();
	}
}

// renders the game hud
void Level::drawHud() {

	// buffer to hold the text to be rendered
	char buffer[256];

	// begin sprite batch
	_hudSpriteBatch.begin();

	// add the text to be rendered 

	// fill buffer with level data
	sprintf_s(buffer, "Level: %d", _levelNum + 1);

	// draw the level
	_spriteFont->draw(_hudSpriteBatch,
		buffer,
		glm::vec2(HUD_POS.x, HUD_POS.y),
		glm::vec2(1),
		0.0f,
		_levelColor);

	// fill buffer with score data
	sprintf_s(buffer, "Score: %d", _score);

	// draw the score
	_spriteFont->draw(_hudSpriteBatch,
		buffer,
		glm::vec2(HUD_POS.x + 280, HUD_POS.y),
		glm::vec2(1),
		0.0f,
		_lastColorDestroyed);

	// end sprite bacth
	_hudSpriteBatch.end();
	// render the sprite batch
	_hudSpriteBatch.renderBatch();

}

// draw level complete
void Level::drawLevelCompleteAnimation() {
	if (_levelComplete) {
		_levelCompleteSpriteBatch.begin();

		// draw the level complete

		// draw level complete animation
		// get destRect
		glm::vec4 destRect(0, BACKGROUND_OFFSET_Y, (float)_screenWidth / _camera.getScale(), (float)_screenHeight / _camera.getScale());
		// get uvRect
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		// draw the level complete animation
		_levelCompleteSpriteBatch.draw(destRect, uvRect, Pengine::ResourceManager::getTexture("../Textures/level_complete.png").id, 1.0f, _levelColor);

		// end sprite batch
		_levelCompleteSpriteBatch.end();
		// render the batch
		_levelCompleteSpriteBatch.renderBatch();
	}
}