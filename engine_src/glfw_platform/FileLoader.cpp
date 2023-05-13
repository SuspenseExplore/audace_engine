#include <fstream>
#include <sstream>
#include "FileLoader.h"

namespace Audace {
	std::string FileLoader::textFileToString(std::string path) {
		std::ifstream fin(basePath + path, std::ios::in);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}
}