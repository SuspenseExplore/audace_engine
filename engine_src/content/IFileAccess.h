#ifndef AU_IFILELOADER_H
#define AU_IFILELOADER_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Audace
{
	class ByteBuffer;
	class ImageData;
	class Model;

	class IFileAccess
	{
	public:
		virtual ByteBuffer* readFileToBuffer(const std::string& path) = 0;
		virtual std::string textFileToString(const std::string& path) = 0;
		virtual json textFileToJson(const std::string& path) = 0;
		virtual ImageData readImageFile(const std::string& path) = 0;
		virtual Model* readModelFile(std::string path, std::string filename) = 0;

		virtual std::string assetReadBasePath() = 0;
		virtual std::string fileWriteBasePath() = 0;

		virtual void setExternalFilePath(std::string path) = 0;
	};
}

#endif