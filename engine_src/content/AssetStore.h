#ifndef AU_ASSETSTORE_H
#define AU_ASSETSTORE_H

#include <string>
#include <map>
#include "FileLoader.h"
#include "renderer/Mesh.h"
#include "renderer/ShaderProgram.h"
#include "renderer/material/BaseMaterial.h"
#include "renderer/material/SimpleBillboardMaterial.h"
#include "renderer/Texture2d.h"
#include "renderer/Sprite.h"

namespace Audace
{
	class AssetStore
	{
		static FileLoader *fileLoader;
		static Mesh *squareMesh;
		static std::map<std::string, ShaderProgram *> shaders;
		static SimpleBillboardMaterial *billboardMat;
		static std::map<std::string, Texture2d *> textures;
		static std::map<std::string, Model *> models;
		static std::map<std::string, Sprite *> sprites;

	public:
		static void init(FileLoader *loader);
		static ShaderProgram *getShader(const std::string &name);
		static Mesh *getSquareMesh() { return squareMesh; }
		static ShaderProgram *simpleTextShader() { return shaders["AU_simple_text"]; }
		static ShaderProgram *simpleBillboardShader() { return shaders["AU_simple_billboard"]; }
		static SimpleBillboardMaterial *simpleBillboardMaterial() { return billboardMat; }
		static Texture2d *getWhiteTexture() { return textures["AU_white_texture"]; }

		static Texture2d *getTexture(const std::string &name);
		static Model *getModel(const std::string &name);
		static Sprite *cloneSprite(const std::string &name);
	};
}

#endif