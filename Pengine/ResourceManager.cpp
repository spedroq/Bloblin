// wrapper class to use the TextureCache
#include "ResourceManager.h"

namespace Pengine {
	TextureCache ResourceManager::_textureCache;


	GLTexture ResourceManager::getTexture(std::string texturePath) {

		return _textureCache.getTexture(texturePath);
	}
}