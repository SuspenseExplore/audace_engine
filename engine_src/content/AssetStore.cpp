#include <sstream>
#include <vector>
#include "AssetStore.h"
#include "renderer/Shapes.h"

namespace Audace
{
	unsigned char WHITE_TEX[] = {255, 255, 255, 255};
	FileLoader *AssetStore::fileLoader;
	Mesh *AssetStore::squareMesh;
	std::map<std::string, ShaderProgram *> AssetStore::shaders;
	SimpleBillboardMaterial *AssetStore::billboardMat;
	std::map<std::string, Texture2d *> AssetStore::textures;
	std::map<std::string, Model *> AssetStore::models;
	std::map<std::string, Sprite *> AssetStore::sprites;

	void AssetStore::init(FileLoader *loader)
	{
		fileLoader = loader;
		squareMesh = Shapes::squarePositions();
		getShader("AU_simple_text");
		getShader("AU_simple_billboard");
		billboardMat = new SimpleBillboardMaterial;
		billboardMat->setShader(simpleBillboardShader());
		billboardMat->setColor(glm::vec4(1, 1, 1, 1));

		ImageData data(&WHITE_TEX[0], 1, 1, GL_RGBA);
		Texture2d *whiteTex = new Texture2d(data);
		whiteTex->create();
		textures["AU_white_texture"] = whiteTex;
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
			Texture2d *tex = new Texture2d(img);
			tex->create();
			textures[name] = tex;
			return tex;
		}
		else
		{
			return textures[name];
		}
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

	Sprite *AssetStore::cloneSprite(const std::string &name)
	{
		if (sprites.find(name) == sprites.end())
		{
			Sprite *sprite = new Sprite(getModel(name));
			sprites[name] = sprite;
		}
		return sprites[name]->clone();
	}

	Sprite *AssetStore::getCubeSprite()
	{
		std::string name = "AU_cube_sprite";
		if (sprites.find(name) == sprites.end())
		{
			std::vector<Audace::Mesh *> v = {Audace::Shapes::cubePosNormTan()};
			Sprite *s = new Sprite(v);
			sprites[name] = s;
		}
		return sprites[name]->clone();
	}
}