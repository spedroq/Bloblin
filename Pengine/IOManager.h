#pragma once

#include <vector>

namespace Pengine {
	// static class IOManager
	// we only need one instance of this, thus the use of static (thus the lack of constructor)
	class IOManager
	{
	public:

		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

	};
}

