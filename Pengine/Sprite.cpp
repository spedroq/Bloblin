#include "Sprite.h"
// uses our defined vertex
#include "Vertex.h"

// helps find offset of
#include <cstddef>

// include the resource manager which is a wrapper for the TextureCache
#include "ResourceManager.h"

namespace Pengine {

	Sprite::Sprite() : _vboID(0)
	{

	}


	Sprite::~Sprite()
	{
		// delete buffer if it exists
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	// init the sprite
	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		// load the texture here
		_texture = ResourceManager::getTexture(texturePath);

		// create the vertex buffer
		// check if it has not been created already (not generated should = 0 as we set it)
		if (_vboID == 0) {
			// generate the vertex buffer and assign it to the id variable created
			// generates 1 buffer, assigns to _vboID
			glGenBuffers(1, &_vboID);
		}

		// vertexData array holds the vertex data
		Vertex vertexData[6];

		// first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		// second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		// set the vertex colours
		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		// change a couple of colors to test shaders
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);
		//

		// bind buffer (tells openGL to have this buffer active)
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		// upload data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// draw the sprite
	void Sprite::draw() {

		// bind texture
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		// bind buffer (tells openGL to have this buffer active)
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		// enables the vertex array in the buffer 
		// the attribute passed is the index of the array to use
		// 0 is the zeroth index, (the array of positions)
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		

		// make it draw
		// mode set to triangles, the first element set to 0, number of vertices set to 6 (it knows there are 2 coordinates)
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// disable vertex after we draw
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}