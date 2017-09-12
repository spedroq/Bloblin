#include "SpriteBatch.h"

#include <algorithm>

namespace Pengine {
	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	// initialize
	void SpriteBatch::init() {

		// create the vertex array and the buffers required
		createVertexArray();


	}

	// begins the process to draw
	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {

		// stop memory leaks by resetting the variables

		// set the type of glyph sorting
		_sortType = sortType;
		// clear the render batches
		_renderBatches.clear();

		// clear the glyphs
		_glyphs.clear();
	}

	// end is called after sprites are added to the batch, post processing (such as sort etc)
	void SpriteBatch::end() {
		// create the list of pointer for faster sorting
		int size = _glyphs.size();
		_glyphPointers.resize(size);
		for (int i = 0; i < size; i++) {
			_glyphPointers[i] = &_glyphs[i];
		}

		// sort the glyph list
		sortGlyphs();
		// create render batches
		createRenderBatches();
	}

	// adds the sprites to the batch after begin is called
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		// push back s nre glyph
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	// renders the batch and draws it after end is called
	void SpriteBatch::renderBatch() {

		// bind array
		glBindVertexArray(_vao);

		int size = _renderBatches.size();
		// loop through render bacthes and draw them all
		for (int i = 0; i < size; i++) {

			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);

		}

		// unbind array
		glBindVertexArray(0);
	}

	// add glyphs to batches of same texture glyph
	void SpriteBatch::createRenderBatches() {

		int numGlyphs = _glyphs.size();
		// vector of vertices
		std::vector<Vertex> vertices;
		// tells the vertices vector, how big it will be. (should be 6 vertices per glyph)
		vertices.resize(numGlyphs * 6); // since there are two triangles

		// check if there are any batches
		if (_glyphs.empty()) {
			return;
		}

		// keep track of the offset
		int offset = 0;

		// current vertex
		int cv = 0;

		// set up the first render batch
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);

		// add vertices to the array and increment the cv index only after usage
		// 1st triangle
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		//2nd triangle
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;

		// increment offset
		offset += 6;

		// search for other occurences of the current glyph
		for (int cg = 1; cg < numGlyphs; cg++) {

			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				// set up a new render batch
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}
			// add vertices to the array and increment the cv index only after usage
			// 1st triangle
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			//2nd triangle
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;

			// increment offset
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// orphan the buffer (so it doesnt have to rewrite over old data)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// create the vertex array
	void SpriteBatch::createVertexArray() {

		// generate the vertex array
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		// bind the vertex array
		glBindVertexArray(_vao);

		// generate the buffer
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		// bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// enables the vertex array in the buffer 
		// the attribute passed is the index of the array to use
		// 0 is the zeroth index, (the array of positions)
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		// tells openGL to where to start drawing from the data we gave it (what to draw)
		// we might not want to draw the entire array
		// in this case we will use the entire array
		// 0 is the array, 2 is for 2d(x,y), data type of the array, false on normalized, 
		// stride same as the size of the struct holding our vertex, 
		// pointer is the byte offset of where the struct is defined within the vertex struct, 
		// in this case position is the first struct defined, thus offset of 0
		//
		// this is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// 1 is the index of the input
		// 4 is the byte size of each value, a Vertex.color has 4 bytes
		// GL_UNSIGNED_BYTE is the type of values
		// GL_TRUE as we want the values to be normalised this time around (converts the colors from 0-255 into a float)
		// we can keep using the more efficient unsigned byte
		// the stride will be the size of Vertex
		// the offset will be 8 bytes in this case, but we can write it more elequentely as follows (in case their position changes)
		//
		// this is the color attrivute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// 2 is the index of the input
		// 2 is the byte size of each value, a Vertex.UV has 2 bytes
		// GL_FLOAT is the type of values
		// GL_FALSE as we dont want the values to be normalised
		// we can keep using the more efficient unsigned byte
		// the stride will be the size of Vertex
		// the offset will be 12 bytes in this case, but we can write it more elequentely as follows (in case their position changes)
		//
		// this is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// unbind
		glBindVertexArray(0);

	}

	// sort the list of glyphs
	void SpriteBatch::sortGlyphs() {
		
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
				break;
			case GlyphSortType::NONE:
				break;
		}
		
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b){
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b){
		return (a->texture < b->texture);
	}
}
