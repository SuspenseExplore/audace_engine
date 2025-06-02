#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <string>
#include <iostream>
#include <vector>
#include "content/IFileLoader.h"
#include "ImageData.h"
#include "content/Model.h"
#include "content/ByteBuffer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class FileLoader : public IFileLoader
	{
		std::string basePath;

	public:
		FileLoader(std::string basePath) : basePath(basePath) {}

		ByteBuffer *readFileToBuffer(const std::string &path);
		std::string textFileToString(std::string path) override;
		json textFileToJson(std::string path) override;
		ImageData readImageFile(std::string path);
		Model *readModelFile(std::string path, std::string filename);

		std::vector<std::string> listFilesInDir(const std::string &path, bool recursive = false);
	};
}

#endif