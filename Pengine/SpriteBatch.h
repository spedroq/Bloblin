#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace Pengine {


	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};


	// a single sprite
	class Glyph {
	public:

		Glyph() {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
			texture(Texture),
			depth(Depth)
		{
			// give it a texture and a depth
			texture = texture;
			depth = depth;

			// add the vertices
			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}

		// texture id
		GLuint texture;

		// depth to be drawn in
		float depth;

		// vertices
		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

	};

	// render batch, stores info on what it needs to render
	class RenderBatch {

	public:
		// constructor
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture){

		}
		// offset
		GLuint offset;
		// vertecis
		GLuint numVertices;
		// texture id
		GLuint texture;

	};

	// sprite batch, makes drawing sprites more efficient
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		// initialize
		void init();

		// begins the process to draw
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE); // set to default type TEXTURE, if no param is sent

		// end is called after sprites are added to the batch, post processing (such as sort etc)
		void end();

		// adds the sprites to the batch after begin is called
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

		// renders the batch and draws it after end is called
		void renderBatch();

	private:
		// create render batches
		void createRenderBatches();
		// create the vertex array and bind
		void createVertexArray();
		// sort the list of glyphs
		void sortGlyphs();

		// sort static functions
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		// glyph sort type
		GlyphSortType _sortType;

		GLuint _vbo;
		GLuint _vao;

		// vector of glyphs
		std::vector<Glyph> _glyphs;
		// vector of glyph pointers for sorting
		std::vector<Glyph*> _glyphPointers;
		// vector of render batches
		std::vector<RenderBatch> _renderBatches;

	};
}


