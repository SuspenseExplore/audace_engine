#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <string>
#include "ImageData.h"

namespace Audace {
	class FileLoader {
		std::string basePath;

		public:
		FileLoader(std::string basePath) : basePath(basePath) {}
		std::string textFileToString(std::string path);
		ImageData readImageFile(std::string path);
	};
}

#endif