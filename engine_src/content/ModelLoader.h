#ifndef AU_MODELLOADER_H
#define AU_MODELLOADER_H

#include <string>
#include <map>

namespace Audace {
	class IFileAccess;
	struct Model;
	class Material;

	class ModelLoader {
	public:
		static Model* loadObj(IFileAccess* fileLoader, std::string path, std::string filename);
		static std::map<std::string, Material*> loadMtl(IFileAccess* fileLoader, std::string path, std::string filename);
	};
}

#endif