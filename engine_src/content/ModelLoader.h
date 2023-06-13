#ifndef AU_MODELLOADER_H
#define AU_MODELLOADER_H

#include <string>
#include <map>
#include "content/Model.h"
#include "FileLoader.h"

namespace Audace {
	class ModelLoader {
		public:
		static Model *loadObj(FileLoader* fileLoader, std::string path, std::string filename);
		static std::map<std::string, Material*> loadMtl(FileLoader* fileLoader, std::string path, std::string filename);
	};
}

#endif