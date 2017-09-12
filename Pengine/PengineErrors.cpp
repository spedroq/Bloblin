#include <Windows.h>
#include "PengineErrors.h"

#include <iostream>
// allows the use of windows, openGL, and i/o
#include <SDL/SDL.h>
#include <string>
#include <cstdlib>

namespace Pengine {
	// print errors and exits
	void fatalError(std::string errorString) {
		// print the error string
		std::cout << errorString << std::endl;
		// wait for input before shutting off the program
		std::cout << "Enter any key to quit..." << std::endl;
		int tmp;
		std::cin >> tmp;
		// shut down SDL (quits SDL systems)
		SDL_Quit();
		// finishes the program with 1
		exit(1);
	}
}