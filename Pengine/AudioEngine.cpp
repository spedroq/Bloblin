#include "AudioEngine.h"

#include "PengineErrors.h"

namespace Pengine {

	// sound effect

	void SoundEffect::play(int loops /* DEFAULT = 0 */, int channel /* DEFAULT = -1 */) {
		// play from the first available music channel (usually 8 music channels available)
		if (Mix_PlayChannel(channel, _chunk, loops) == -1) {
			// override sound in channel 1
			if (Mix_PlayChannel(1, _chunk, loops) == -1) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}


	// music

	void Music::play(int loops /* DEFAULT = 1 */) {
		// only one music can play at a time
		if (Mix_PlayMusic(_music, loops) == -1) {
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
		}
	}

	void Music::pause() {
		Mix_PauseMusic();
	}

	void Music::stop() {
		Mix_HaltMusic();
	}

	void Music::resume() {
		Mix_ResumeMusic();
	}



	// audio engine

	AudioEngine::AudioEngine()
	{
		// empty
	}


	AudioEngine::~AudioEngine()
	{
		// clean the audio engine memory
		destroy();
	}

	// initialize the audio engine
	void AudioEngine::init() {
		if (_isInitialized) {
			fatalError("Tried to init audio engine twice!\n");
		}
		else {
			// parameter can be a bitwise combination of 
			// MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
			// in order to support certain types of music files
			if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
				fatalError("Mix_Init error: " + std::string(Mix_GetError()));
			}
			// init the audio
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)) {
				fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
			}

		}
		_isInitialized = true;
	}

	// destroy the audio engine
	void AudioEngine::destroy() {

		if (_isInitialized) {
			// set is initialized to false to avoid multiple ddestroy calls
			_isInitialized = false;

			// free up all the the sound effects
			for (auto& it : _soundEffectCache){
				Mix_FreeChunk(it.second);
			}
			// free up all the the music
			for (auto& it : _musicCache) {
				Mix_FreeMusic(it.second);
			}
			// clear the caches
			_soundEffectCache.clear();
			_musicCache.clear();

			// close the audio
			Mix_CloseAudio();
			// quit mix
			Mix_Quit();
		}
	}

	// loads a sound effect
	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		// iterate through sound effect cache to see if it has already been loaded
		auto it = _soundEffectCache.find(filePath);

		SoundEffect effect;

		if (it == _soundEffectCache.end()) {
			// failed to find it
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			// successfully loaded it thus place it in the cache
			effect._chunk = chunk;
			_soundEffectCache[filePath] = chunk;
		}
		else {
			// found the sound effect already cached thus point to the loaded chunk
			effect._chunk = it->second;
		}

		return effect;

	}

	// loads a music
	Music AudioEngine::loadMusic(const std::string& filePath) {
		// iterate through music cache to see if it has already been loaded
		auto it = _musicCache.find(filePath);

		Music music;

		if (it == _musicCache.end()) {
			// failed to find it
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			// successfully loaded it thus place it in the cache
			music._music = mixMusic;
			_musicCache[filePath] = mixMusic;
		}
		else {
			// found the music already cached thus point to the loaded music
			music._music = it->second;
		}

		return music;
	}

}