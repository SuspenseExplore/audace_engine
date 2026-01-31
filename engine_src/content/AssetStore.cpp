#include <sstream>
#include <vector>
#include "AssetStore.h"
#include "renderer/Shapes.h"
#include "content/IFileAccess.h"
#include "content/Model.h"
#include "content/gltf/GltfLoader.h"
#include "renderer/Mesh.h"
#include "renderer/texture/Texture2d.h"
#include "renderer/texture/TextureCubemap.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "scene/graph/SceneGraphNode.h"
#include "util/StringUtil.h"

namespace Audace
{
	unsigned char WHITE_TEX[] = {255, 255, 255, 255};
	unsigned char BLUE_TEX[] = {0, 0, 255, 255};
	unsigned char BLACK_TEX[] = {0, 0, 0, 255};
	IFileAccess *AssetStore::fileLoader;
	Mesh *AssetStore::squareMesh;
	std::map<std::string, ShaderProgram *> AssetStore::shaders;
	SimpleBillboardMaterial *AssetStore::billboardMat;
	std::map<std::string, ITexture *> AssetStore::textures;
	std::map<std::string, Model *> AssetStore::models;
	std::map<std::string, Sprite *> AssetStore::sprites;
	std::map<std::string, SceneGraphNode *> AssetStore::nodes;

	void AssetStore::init(IFileAccess *loader)
	{
		fileLoader = loader;
		squareMesh = Shapes::squarePositions();
		getShader("AU_debug");
		getShader("AU_simple_text");
		getShader("AU_simple_billboard");
		billboardMat = new SimpleBillboardMaterial;
		billboardMat->setShader(simpleBillboardShader());
		billboardMat->setColor(glm::vec4(1, 1, 1, 1));

		{
			ShaderProgram *shader = getShader("AU_axes");
			BaseMaterial *mat = new SimpleBillboardMaterial();
			mat->setShader(shader);
			Mesh *mesh = Shapes::coloredAxes();
			Sprite *sprite = new Sprite({mesh});
			mesh->setMaterial(mat);
			sprites["AU_axes"] = sprite;
		}

		{
			ImageData data(&WHITE_TEX[0], 1, 1, GL_RGBA);
			Texture2d *whiteTex = new Texture2d(data);
			whiteTex->create();
			textures["AU_white_texture"] = whiteTex;
			billboardMat->setTexture(whiteTex);
		}
		{
			ImageData data(&BLUE_TEX[0], 1, 1, GL_RGBA);
			Texture2d *blueTex = new Texture2d(data);
			blueTex->create();
			textures["AU_blue_texture"] = blueTex;
		}
		{
			ImageData data(&BLACK_TEX[0], 1, 1, GL_RGBA);
			Texture2d *blackTex = new Texture2d(data);
			blackTex->create();
			textures["AU_black_texture"] = blackTex;
		}
	}

	ShaderProgram *AssetStore::getShader(const std::string &name)
	{
		if (shaders.find(name) == shaders.end())
		{
			std::stringstream ss;
			ss << "shaders/" << name << "/vs.glsl";
			std::string vs = fileLoader->textFileToString(ss.str());
			ss.str(std::string());
			ss << "shaders/" << name << "/fs.glsl";
			std::string fs = fileLoader->textFileToString(ss.str());
			ShaderProgram *shaderProgram = new ShaderProgram(vs, fs);
			shaderProgram->create();
			shaders[name] = shaderProgram;
			return shaderProgram;
		}
		else
		{
			return shaders[name];
		}
	}

	Texture2d *AssetStore::getTexture(const std::string &name)
	{
		if (textures.find(name) == textures.end())
		{
			ImageData img = fileLoader->readImageFile(name);
			ITexture *tex = new Texture2d(img);
			tex->create();
			textures[name] = tex;
			return reinterpret_cast<Texture2d *>(tex);
		}
		else
		{
			return reinterpret_cast<Texture2d *>(textures[name]);
		}
	}

	Texture2d *AssetStore::getHdrTexture(const std::string &name)
	{
		if (textures.find(name) == textures.end())
		{
			ImageData img = fileLoader->readHdrImageFile(name);
			ITexture *tex = new Texture2d(img);
			tex->create();
			textures[name] = tex;
			return reinterpret_cast<Texture2d *>(tex);
		}
		else
		{
			return reinterpret_cast<Texture2d *>(textures[name]);
		}
	}

	TextureCubemap *AssetStore::getCubemapTex(const std::string &name)
	{
		static std::string filenames[] = {"nx.png", "px.png", "pz.png", "nz.png", "py.png", "ny.png"};
		if (textures.find(name) == textures.end())
		{
			std::vector<ImageData> imgs;
			imgs.resize(6);
			for (int i = 0; i < 6; i++)
			{
				std::string realName = name + "/" + filenames[i];
				imgs[i] = fileLoader->readImageFile(realName);
			}
			TextureCubemap *tex = new TextureCubemap(imgs);
			tex->create();
			textures[name] = tex;
			return tex;
		}
		else
		{
			return reinterpret_cast<TextureCubemap *>(textures[name]);
		}
	}

	TextureCubemap *AssetStore::getCubemapHdrTex(const std::string &name)
	{
		static std::string filenames[] = {"px.hdr", "nx.hdr", "pz.hdr", "nz.hdr", "ny.hdr", "py.hdr"};
		if (textures.find(name) == textures.end())
		{
			std::vector<ImageData> imgs;
			imgs.resize(6);
			for (int i = 0; i < 6; i++)
			{
				std::string realName = name + "/" + filenames[i];
				imgs[i] = fileLoader->readHdrImageFile(realName);
			}
			TextureCubemap *tex = new TextureCubemap(imgs);
			tex->create();
			textures[name] = tex;
			return tex;
		}
		else
		{
			return reinterpret_cast<TextureCubemap *>(textures[name]);
		}
	}

	Texture2d *AssetStore::darkGridTexture()
	{
		return getTexture("images/dark_grid.png");
	}

	Model *AssetStore::getModel(const std::string &name)
	{
		if (models.find(name) == models.end())
		{
			Model *model = fileLoader->readModelFile("models/", name);
			models[name] = model;
			return model;
		}
		else
		{
			return models[name];
		}
	}

	Sprite *AssetStore::getSprite(const std::string &name)
	{
		if (sprites.find(name) != sprites.end())
		{
			return sprites[name];
		}

		if (StringUtil::endsWith(name, ".obj"))
		{
			Model *model = getModel(name);
			Sprite *sprite = new Sprite(model);
			sprite->setName(name);
			sprites[name] = sprite;
			return sprite;
		}

		return nullptr;
	}

	SceneGraphNode *AssetStore::getGltfNode(const std::string &name)
	{
		if (nodes.find(name) != nodes.end())
		{
			return nodes[name]->clone(true);
		}

		if (StringUtil::endsWith(name, ".gltf") || StringUtil::endsWith(name, ".glb"))
		{
			std::vector<std::string> v = StringUtil::splitFilePath(name);
			GltfLoader loader;
			loader.setImageLoadPath(v[0]);
			loader.loadFile(fileLoader, v[0], v[1]);
			SceneGraphNode *node = loader.getSceneRootNode(0);
			nodes[name] = node;
			return node->clone(true);
		}
		return nullptr;
	}

	Sprite *AssetStore::getCubeSprite()
	{
		std::string name = "AU_cube_sprite";
		if (sprites.find(name) == sprites.end())
		{
			std::vector<Audace::Mesh *> v = {Audace::Shapes::cubePosNormTan()};
			SimpleBillboardMaterial *mat = new SimpleBillboardMaterial();
			mat->setShader(simpleBillboardShader());
			v[0]->setMaterial(mat);
			Sprite *s = new Sprite(v);
			sprites[name] = s;
		}
		return sprites[name]->clone();
	}

	Sprite *AssetStore::getColoredAxes()
	{
		return sprites["AU_axes"];
	}
}