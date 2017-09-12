#include "PengineWindow.h"
#include "PengineErrors.h"

#include <SDL/SDL_image.h>

namespace Pengine {
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		// window flags
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		// create the window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		// check for errors
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		// context variable
		// set it up and give it to the _window
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		// check for errors
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		// inits glew, setup glew and get all the extentions set up
		// also do error checking
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Coud not initialize glew!");
		}

		// check OpenGL version
		std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		// set background
		// this function sets the background color to be drawn
		// when glClear is called over GL_COLOR_BUFFER_BIT
		glClearColor(0.243f, 0.537f, 0.729f, 1.0);

		// set VSync on (1) (off is 0)
		SDL_GL_SetSwapInterval(1);

		// enable alpha blending (for transparency)
		glEnable(GL_BLEND);
		// blend the alpha color from the source with the background
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SDL_Surface* icon = IMG_Load("../icon/icon.png");

		SDL_SetWindowIcon(_sdlWindow, icon);

		return 0;
	}


	void Window::swapBuffer() {
		// swap frame (flushes everything we had drawn to the screen)
		SDL_GL_SwapWindow(_sdlWindow);
	}
}