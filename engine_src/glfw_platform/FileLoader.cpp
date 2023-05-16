#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "FileLoader.h"

namespace Audace {
	std::string FileLoader::textFileToString(std::string path) {
		std::ifstream fin(basePath + path, std::ios::in);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}

	ImageData FileLoader::readImageFile(std::string path) {
		int width;
		int height;
		unsigned char *bytes = stbi_load((basePath + path).c_str(), &width, &height, nullptr, 0);
		ImageData img(bytes, width, height, GL_RGB);
		return img;
	}
}