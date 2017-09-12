#pragma once

#include <SDL/SDL_mixer.h>

#include <string>
#include <map>

namespace Pengine {

	// sound effect wrapper for mixed chunk
	class SoundEffect {
	public: 
		// allows audio engine to use private variables
		friend class AudioEngine;

		// plays sound effect
		// @param loops: if loops == -1, loop forever, otherwise loop for loops + 1 times
		void play(int loops = 0, int channel = -1);

	private:
		Mix_Chunk* _chunk = nullptr;
	};

	// music wrapper for mixed music
	class Music {
	public:
		// allows audio engine to use private variables
		friend class AudioEngine;

		// plays music
		// @param loops: if loops == -1, loop forever, otherwise loop for loops times
		void play(int loops = 1);

		// these can all be static since there is only one music play at a time
		// pause music
		static void pause();
		// stop music
		static void stop();
		// resume music
		static void resume();

	private:
		Mix_Music* _music = nullptr;
	};

	


	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		// initialize the audio engine
		void init();
		// destroy the audio engine
		void destroy();

		// loads a sound effect
		SoundEffect loadSoundEffect(const std::string& filePath);

		// loads a music
		Music loadMusic(const std::string& filePath);

	private:
		// sound effect cache
		std::map<std::string, Mix_Chunk*> _soundEffectCache;
		// music cache
		std::map<std::string, Mix_Music*> _musicCache;


		bool _isInitialized = false;
	};

}