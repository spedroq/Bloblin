#include "InputManager.h"

namespace Pengine {
	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update() {
		// iterate through current key maps and save them in previous key map
		// using a for each loop
		for (auto& it : _keyMap) {
			// copy over the current key to the map of previous keys
			_previousKeyMap[it.first] = it.second;
		}

	}

	// press the key corresponding to the keyID
	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	// release the key corresponding to the keyID
	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	// check if the key is pressed
	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		// check if the item was found
		if (it != _keyMap.end()) {
			// return the boolean value (second stored value)
			return it->second;
		}
		else {
			// not found
			return false;
		}

	}

	// check if the key is pressed
	bool InputManager::isKeyPressed(unsigned int keyID) {

		// check if key is currently pressed, and if it was not pressed before
		if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
			return true;
		}
		return false;

	}

	// check if the key is pressed
	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto it = _previousKeyMap.find(keyID);
		// check if the item was found
		if (it != _previousKeyMap.end()) {
			// return the boolean value (second stored value)
			return it->second;
		}
		else {
			// not found
			return false;
		}

	}

	// set the mouse coordinates
	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}

