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
		virtual std::string textFileToString(std::string path) = 0;
		virtual json textFileToJson(std::string path) = 0;
	};
}

#endif