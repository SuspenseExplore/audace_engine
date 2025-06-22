#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <string>
#include <iostream>
#include <vector>
#include "content/IFileAccess.h"
#include "ImageData.h"
#include "content/Model.h"
#include "content/ByteBuffer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class FileAccessGlfw : public IFileAccess
	{
		std::string externalFilePath;

	public:
		FileAccessGlfw() {}

		ByteBuffer* readFileToBuffer(const std::string& path) override;
		std::string textFileToString(const std::string& path) override;
		json textFileToJson(const std::string& path) override;
		ImageData readImageFile(const std::string& path) override;
		Model* readModelFile(std::string path, std::string filename) override;

		std::vector<std::string> listFilesInDir(const std::string& path, bool recursive = false);

		std::string assetReadBasePath() override { return externalFilePath; }
		std::string fileWriteBasePath() override { return externalFilePath; }

		void setExternalFilePath(std::string path) { externalFilePath = path; }
	};
}

#endif