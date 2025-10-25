#ifndef AU_ASSETSTORE_H
#define AU_ASSETSTORE_H

#include <string>
#include <map>
#include "renderer/material/SimpleBillboardMaterial.h"
#include "renderer/texture/ITexture.h"
#include "renderer/texture/Texture2d.h"
#include "renderer/texture/TextureCubemap.h"

namespace Audace
{
	class IFileAccess;
	class Mesh;
	class ShaderProgram;
	class Sprite;
	class Model;
	class SceneGraphNode;

	class AssetStore
	{
		static IFileAccess *fileLoader;
		static Mesh *squareMesh;
		static std::map<std::string, ShaderProgram *> shaders;
		static SimpleBillboardMaterial *billboardMat;
		static std::map<std::string, ITexture *> textures;
		static std::map<std::string, Model *> models;
		static std::map<std::string, Sprite *> sprites;
		static std::map<std::string, SceneGraphNode *> nodes;

	public:
		static void init(IFileAccess *loader);
		static ShaderProgram *getShader(const std::string &name);
		static Mesh *getSquareMesh() { return squareMesh; }
		static Sprite *getCubeSprite();
		static Sprite *getColoredAxes();
		static ShaderProgram *debugShader() { return shaders["AU_debug"]; }
		static ShaderProgram *simpleTextShader() { return shaders["AU_simple_text"]; }
		static ShaderProgram *simpleBillboardShader() { return shaders["AU_simple_billboard"]; }
		static SimpleBillboardMaterial *simpleBillboardMaterial() { return billboardMat; }
		static Texture2d *getWhiteTexture() { return reinterpret_cast<Texture2d *>(textures["AU_white_texture"]); }
		static Texture2d *getBlueTexture() { return reinterpret_cast<Texture2d *>(textures["AU_blue_texture"]); }
		static Texture2d *getBlackTexture() { return reinterpret_cast<Texture2d *>(textures["AU_black_texture"]); }
		static Texture2d *darkGridTexture();

		static Texture2d *getTexture(const std::string &name);
		static TextureCubemap *getCubemapTex(const std::string &name);
		static Model *getModel(const std::string &name);
		static Sprite *getSprite(const std::string &name);
		static SceneGraphNode *getGltfNode(const std::string &name);
	};
}

#endif