// Holds timing functionality

#include "Timing.h"

#include <SDL/SDL.h>

namespace Pengine {

	FpsLimiter::FpsLimiter() {

	}

	void FpsLimiter::init(float maxFPS) {

		// set the target FPS
		setTargetFPS(maxFPS);
	}

	void FpsLimiter::setTargetFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}

	// begin FPS Counter/Limiter
	void FpsLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}

	// end FPS Counter/Limiter
	// returns the current FPS
	float FpsLimiter::end() {
		// calculate FPS
		calculateFPS();

		// END frame time measuring
		float frameTicks = SDL_GetTicks() - _startTicks;

		// Limit FPS to the set max FPS
		// desired game time (1ms/s) / (f/s)
		float desiredGameTime = (1000.0f / _maxFPS);
		if (desiredGameTime > frameTicks) {
			SDL_Delay(desiredGameTime - frameTicks);
		}

		return _fps;
	}


	// calculate fps
	void FpsLimiter::calculateFPS() {

		// the number of samples can be constant
		static const int NUM_SAMPLES = 10;
		// frame time samples to average out
		static float frameTimes[NUM_SAMPLES];
		// current frame
		static int currentFrame = 0;


		// it will only be init once since it is static
		static float prevTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		// first time around the frameTime = 0
		// but from then on the frameTime will give us the time of the frame because of the static variable
		_frameTime = currentTicks - prevTicks;

		// circular usage of the frameTimes array
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		// calculate number of frame times to average
		int count;
		if (currentFrame < NUM_SAMPLES) {
			// first time around the array index will be the same as the number of values
			count = currentFrame;
		}
		else {
			// from the point the array is full, due to circular usage the count will be the array size
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}

		// check division by 0
		if (count > 0) {
			frameTimeAverage /= count;
		}
		else {
			// this should only happen on the first frame time
			frameTimeAverage = 0;
		}

		// check division by 0
		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			// this should only happen on the first frame time
			_fps = 70.0f;
		}

		// UPDATE variables for next usage
		// increment frame int
		currentFrame++;
		// update previousTicks after being used
		prevTicks = currentTicks;
	}


}