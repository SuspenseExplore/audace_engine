#ifndef AU_IFILELOADER_H
#define AU_IFILELOADER_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Audace
{
	class IFileLoader
	{
	public:
		virtual std::string textFileToString(const std::string& path) = 0;
		virtual json textFileToJson(const std::string& path) = 0;

		virtual std::string assetReadBasePath() = 0;
		virtual std::string fileWriteBasePath() = 0;

		virtual void setExternalFilePath(std::string path) = 0;
	};
}

#endif