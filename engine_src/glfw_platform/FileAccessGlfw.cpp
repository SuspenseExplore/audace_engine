#include <fstream>
#include <sstream>
#include <Windows.h>
#include "stb_image.h"
#include "au_renderer.h"
#include "FileAccessGlfw.h"
#include "content/ByteBuffer.h"
#include "content/ImageData.h"
#include "content/Model.h"
#include "content/ModelLoader.h"

namespace Audace
{
	ByteBuffer* FileAccessGlfw::readFileToBuffer(const std::string& path)
	{
		std::ifstream fin(fileWriteBasePath() + path, std::ios::in | std::ios::binary | std::ios::ate);
		int size = fin.tellg();
		fin.seekg(0);
		char* buf = new char[size];
		fin.read(buf, size);
		return new ByteBuffer(buf, size);
	}

	std::string FileAccessGlfw::textFileToString(const std::string& path)
	{
		AU_ENGINE_LOG_DEBUG("{}", fileWriteBasePath() + path);
		std::ifstream fin(fileWriteBasePath() + path, std::ios::in);
		std::stringstream ss;
		ss << fin.rdbuf();
		fin.close();
		return ss.str();
	}

	json FileAccessGlfw::textFileToJson(const std::string& path)
	{
		std::string s = textFileToString(path);
		return json::parse(s);
	}

	ImageData FileAccessGlfw::readImageFile(const std::string& path)
	{
		int width;
		int height;
		int channels;
		unsigned char* bytes = stbi_load((fileWriteBasePath() + path).c_str(), &width, &height, &channels, 0);
		int format = GL_RGBA;
		switch (channels)
		{
		case 1:
			format = GL_RED;
			break;

		case 3:
			format = GL_RGB;
		}
		ImageData img(bytes, width, height, format);
		return img;
	}

	Model* FileAccessGlfw::readModelFile(std::string path, std::string filename)
	{
		// filename could have part of the path, so put them together and them take them apart again
		std::string fullName = path + filename;
		int i = fullName.find_last_of('/') + 1;
		Model* model = ModelLoader::loadObj(this, fullName.substr(0, i), fullName.substr(i));
		return model;
	}

	std::vector<std::string> FileAccessGlfw::listFilesInDir(const std::string& path, bool recursive)
	{
		std::string searchPath = fileWriteBasePath() + path + "/*";
		std::vector<std::string> filenames;
		WIN32_FIND_DATA data;
		HANDLE handle = ::FindFirstFile(searchPath.c_str(), &data);
		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					filenames.push_back(path + "/" + data.cFileName);
				}
			} while (::FindNextFile(handle, &data));
			::FindClose(handle);
		}

		if (recursive)
		{
			handle = ::FindFirstFile(searchPath.c_str(), &data);
			if (handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
						data.cFileName[0] != '.')
					{
						std::vector<std::string> r = listFilesInDir(path + "/" + data.cFileName, true);
						filenames.insert(filenames.end(), r.begin(), r.end());
					}
				} while (::FindNextFile(handle, &data));
				::FindClose(handle);
			}
		}

		return filenames;
	}
}