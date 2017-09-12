#include "ImageLoader.h"
#include "IOManager.h"
#include "picoPNG.h"
#include "PengineErrors.h"

namespace Pengine {
	GLTexture ImageLoader::loadPNG(std::string filePath) {
		GLTexture texture = {};

		// input from the image file
		std::vector<unsigned char> in;
		// output decode PNG, byte representation of the PNG
		std::vector<unsigned char> out;
		// dimentions
		unsigned long width, height;

		// static use of IOManager to read the file to buffer
		if (IOManager::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file to buffer!");
		}

		// decode PNG
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		// error check
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}

		// generate the OpenGL texture object (generates the id)
		glGenTextures(1, &(texture.id));
		// bind the texture id to the texture object
		glBindTexture(GL_TEXTURE_2D, texture.id);
		// upload the image data to the texture
		// fill texture with image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// generate the mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		// unbind current texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// set the width and height
		texture.width = width;
		texture.height = height;

		return texture;
	}
}