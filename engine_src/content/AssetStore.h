#ifndef AU_ASSETSTORE_H
#define AU_ASSETSTORE_H

#include <string>
#include <map>
#include "FileLoader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/material/BaseMaterial.h"
#include "renderer/material/SimpleBillboardMaterial.h"
#include "renderer/Texture2d.h"

namespace Audace {
	class AssetStore {
		static FileLoader *fileLoader;
		static std::map<std::string, ShaderProgram*> shaders;
		static SimpleBillboardMaterial *billboardMat;
		static std::map<std::string, Texture2d*> textures;

		public:
		static void init(FileLoader *loader);
		static ShaderProgram *getShader(const std::string& name);
		static ShaderProgram* simpleTextShader() {return shaders["AU_simple_text"];}
		static ShaderProgram* simpleBillboardShader() {return shaders["AU_simple_billboard"];}
		static SimpleBillboardMaterial* simpleBillboardMaterial() {return billboardMat;}

		static Texture2d* getTexture(const std::string& name);
	};
}

#endif