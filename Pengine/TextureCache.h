#pragma once

// include map to use maps/dictionaries for efficient searching
#include <map>

#include "GLTexture.h"

namespace Pengine {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};
}

