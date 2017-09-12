#pragma once

#include <GL/glew.h>

namespace Pengine {
	// through the use of struct we can call position.x or y rather than position[0] or [1] (if using an array instead)
	struct Position { // 2D position = 2 coordinates
		float x; // x vertex coordinate
		float y; // y vertex coordinate
	};

	/*
	// float color[3]; // takes too many bits, we don't need floats
	using unsigned char will make it more efficient as the CPU transfer will be much faster
	this will be enough for the colour values, as these are simply in the range of 0-255
	4 component color array (Red, Green, Blue, Alpha (transparency))
	*/
	struct ColorRGBA8 {
		// default constructor
		ColorRGBA8() :
			r(0),
			g(0),
			b(0),
			a(0)
		{
		}
		// constructor with params
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			r(R),
			g(G),
			b(B),
			a(A)
		{
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	// UV holds the UV coordinates for the texture
	// number between 0 and 1 for normal texture mapping. If UV > 1 then tiling is possible
	struct UV {
		float u;
		float v;
	};

	// This Vertex struct holds the data needed for one vertex. (position coordinates and colour)
	// for allignment issues it is reccomended to have a vertex byte size multiple of 4
	// each float has 4 bytes, each GLUbyte has 1 byte
	struct Vertex {

		// create an isntance of Position named position
		Position position;
		// create an isntance of Color named color
		ColorRGBA8 color;

		// UV texture coordinates
		UV uv;

		// set position
		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		// set the color
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		// set UV
		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}