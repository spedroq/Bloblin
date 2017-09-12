
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Pengine.h"

namespace Pengine {
	int init() {
		// initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// sets two buffers, so we draw frame A and B
		// if we draw to the A buffer, we put it on the screen
		// next frame we swap to B and start writing to B, which will clear A
		// next frame swap again clearing B and using A etc.
		// used right before creating the window
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
