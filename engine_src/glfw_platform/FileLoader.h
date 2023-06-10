#ifndef AU_FILELOADER_H
#define AU_FILELOADER_H

#include <string>
#include <iostream>
#include "ImageData.h"
#include "content/Model.h"

namespace Audace {
	class FileLoader {
		std::string basePath;

		public:
		FileLoader(std::string basePath) : basePath(basePath) {}

		std::string textFileToString(std::string path);
		ImageData readImageFile(std::string path);
		Model* readModelFile(std::string path, std::string filename);
	};
}

#endif