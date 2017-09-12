#include "TextureCache.h"

#include "ImageLoader.h"

#include <iostream>

namespace Pengine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}


	GLTexture TextureCache::getTexture(std::string texturePath) {

		// look up the texture in the texture map
		//std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);
		auto mit = _textureMap.find(texturePath);

		// check the texture is not in the map
		if (mit == _textureMap.end()) {
			// means there is no instances of that texture in the map
			// thus load the new texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// create the key value pair
			//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			// insert the new pair into the texture map
			_textureMap.insert(make_pair(texturePath, newTexture)); // alternatively can be done in one line

			return newTexture;
		}

		// this means the texture has already been loaded into the texture map
		// mit->first is the key, mit->second is the value
		return mit->second;
	}
}