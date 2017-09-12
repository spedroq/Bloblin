#include "IOManager.h"

#include <fstream>

namespace Pengine {
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {

		std::ifstream file(filePath, std::ios::binary);

		// error check if file was successfully read
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// seek to end of file, and tell the size
		file.seekg(0, std::ios::end);
		int fileSize = file.tellg();

		// seek back to the begining
		file.seekg(0, std::ios::beg);

		// reduce the file size by any header bytes that might be present
		// take away the file header from the byte count (in case there is one)
		fileSize -= file.tellg();

		buffer.resize(fileSize);

		// read the file into the buffer
		// takes char* thus need the address of the first position of our vector
		file.read((char *) &(buffer[0]), fileSize);

		// close the file (although fstreams destroy themselves)
		file.close();

		return true;
	}
}