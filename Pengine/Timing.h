#pragma once

namespace Pengine {

	class FpsLimiter {

	public:
		FpsLimiter();
		
		void init(float _maxFPS);

		// set the target FPS
		void setTargetFPS(float _maxFPS);

		// begin FPS Counter/Limiter
		void begin();
		// end FPS Counter/Limiter
		// returns the current FPS
		float end();

	private:

		// calculates the FPS
		void calculateFPS();


		// FPS counters
		// fps
		float _fps;
		// frame time
		float _frameTime;
		// holds the maximum FPS (target FPS)
		float _maxFPS;
		// holds the start frame ticks
		unsigned int _startTicks;

	};

}