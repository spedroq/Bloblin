#pragma once
// Window abstracts the OpenGL and SDL specifics from the Game

// include SDL
#include <SDL/SDL.h>
// include OpenGL glew
#include <GL/glew.h>
//include string
#include <string>

namespace Pengine {

	// give them bitwise values that can be passed together
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		// init window
		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		// accessors
		int getScreenWidth() {
			return _screenWidth;
		}
		int getScreenHeight() {
			return _screenHeight;
		}
		// mutators
		void setScreenWidth(int width) {
			_screenWidth = width;
		}
		void setScreenHeight(int height) {
			_screenHeight = height;
		}

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};

}