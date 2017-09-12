#pragma once
// include the texture cache to cache the textures that will be used
#include "TextureCache.h"

#include <string>

namespace Pengine {
	class ResourceManager
	{
	public:

		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};
}
