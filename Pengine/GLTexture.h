#pragma once

#include <GL/glew.h>

namespace Pengine {
	struct GLTexture {
		// all that is needed is the id of the texture
		GLuint id;

		// usefull variables
		// texture dimentions
		int width;
		int height;
	};
}