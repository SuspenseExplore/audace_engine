#include <fstream>
#include <sstream>
#include <Windows.h>
#include "stb_image.h"
#include "FileLoader.h"
#include "content/ModelLoader.h"

namespace Audace
{
	ByteBuffer *FileLoader::readFileToBuffer(const std::string &path)
	{
		std::ifstream fin(basePath + path, std::ios::in | std::ios::binary | std::ios::ate);
		int size = fin.tellg();
		fin.seekg(0);
		char *buf = new char[size];
		fin.read(buf, size);
		return new ByteBuffer(buf, size);
	}

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

	Model *FileLoader::readModelFile(std::string path, std::string filename)
	{
		Model *model = ModelLoader::loadObj(this, path, filename);
		return model;
	}

	std::vector<std::string> FileLoader::listFilesInDir(const std::string &path)
	{
		std::vector<std::string> filenames;
		WIN32_FIND_DATA data;
		HANDLE handle = ::FindFirstFile((basePath + path).c_str(), &data);
		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					filenames.push_back(data.cFileName);
				}
			} while (::FindNextFile(handle, &data));
			::FindClose(handle);
		}

		return filenames;
	}
}