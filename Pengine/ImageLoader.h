#pragma once

#include "GLTexture.h"
#include <string>

namespace Pengine {
	class ImageLoader
	{
	public:
		// loads the PNG Texture/Sprite
		static GLTexture loadPNG(std::string filePath);
	};
}

