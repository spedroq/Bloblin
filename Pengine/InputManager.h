#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace Pengine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		// press a key
		void pressKey(unsigned int keyID);
		// release a key
		void releaseKey(unsigned int keyID);
		// check if the key is being held
		bool isKeyDown(unsigned int keyID);
		// check if the key was just pressed
		bool isKeyPressed(unsigned int keyID);



		// mutators
		// set the mouse coordinates
		void setMouseCoords(float x, float y);

		// accessors
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		// holds the keys and presses
		std::unordered_map<unsigned int, bool> _keyMap;
		// holds the keys and presses from last frame
		std::unordered_map<unsigned int, bool> _previousKeyMap;
		// holds mouse coordinates
		glm::vec2 _mouseCoords;

		// check if the key was being held
		bool wasKeyDown(unsigned int keyID);

	};
}



