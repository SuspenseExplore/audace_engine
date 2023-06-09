#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "FileLoader.h"
#include "content/ModelLoader.h"

namespace Audace
{
	std::string FileLoader::textFileToString(std::string path)
	{
		std::ifstream fin(basePath + path, std::ios::in);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}

	ImageData FileLoader::readImageFile(std::string path)
	{
		int width;
		int height;
		unsigned char *bytes = stbi_load((basePath + path).c_str(), &width, &height, nullptr, 0);
		ImageData img(bytes, width, height, GL_RGB);
		return img;
	}

	Model *FileLoader::readModelFile(std::string path)
	{
		std::string objExt = ".obj";
		if (std::equal(objExt.rbegin(), objExt.rend(), path.rbegin()))
		{
			std::string content = textFileToString(path);
			Model *model = ModelLoader::loadObj(content);
			return model;
		}
		return nullptr;
	}
}