#ifndef AU_ASSETSTORE_H
#define AU_ASSETSTORE_H

#include <string>
#include <map>
#include "FileLoader.h"
#include "renderer/ShaderProgram.h"

namespace Audace {
	class AssetStore {
		static FileLoader *fileLoader;
		static std::map<std::string, ShaderProgram*> shaders;

		public:
		static void init(FileLoader *loader);
		static ShaderProgram *getShader(const std::string& name);
		static ShaderProgram* simpleTextShader() {return shaders["AU_simple_text"];}
		static ShaderProgram* simpleBillboardShader() {return shaders["AU_simple_billboard"];}
	};
}

#endif