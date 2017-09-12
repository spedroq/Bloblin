#pragma once
#include <Windows.h>
// including glew.h sets up glew and includes all the other files needed for openGL
#include <GL/glew.h>

// include the texture in the sprite
#include "GLTexture.h"

#include <string>

namespace Pengine {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		// init the sprite
		void init(float x, float y, float width, float height, std::string texturePath);
		// draw the sprite
		void draw();

	private:
		// sprite coordinates
		float _x;
		float _y;
		// sprite size
		float _width;
		float _height;

		// vertex buffer object id
		// same type as unisgned int, GLuint is guaranteed to have 32 bit unsigned int
		GLuint _vboID;

		// sprite texture
		GLTexture _texture;
	};

}