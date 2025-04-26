#include "au_renderer.h"
#include "ProcTerrainScene.h"
#include "SceneEnum.h"
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Mesh.h"
#include "FastNoiseLite.h"

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

void ProcTerrainScene::loadAssets(Audace::FileLoader *fileLoader)
{
	material = new Audace::Material;
	material->setName("groundMat");
	material->setShader(Audace::AssetStore::getShader("terrain"));
	material->setAmbientColor({1.0, 1.0, 1.0});
	material->setDiffuseColor({1.0, 1.0, 1.0});
	material->setSpecularColor({0.5, 0.5, 0.5});
	material->setEmissionColor({0.0, 0.0, 0.0});
	material->setAmbientOcclusionMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_AmbientOcclusion.jpg"));
	material->setDiffuseMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Color.jpg"));
	material->setNormalMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_NormalGL.jpg"));
	material->setSpecularMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Roughness.jpg"));

	generateTerrain();
}

void ProcTerrainScene::generateTerrain()
{
	std::vector<Audace::Mesh*> meshes;
	for (int x = -2; x < 2; x++)
	{
		for (int y = -2; y < 2; y++)
		{
			for (int z = -1; z < 2; z++)
			{
				Audace::Mesh *mesh = terrainGen.genChunk({x, y, z});
				mesh->setMaterial(material);
				meshes.push_back(mesh);
			}
		}
	}

	cubeSprite = new Audace::Sprite(meshes);
}

void ProcTerrainScene::addToBuffer(std::vector<float> &buf, glm::vec3 vec)
{
	buf.push_back(vec.x);
	buf.push_back(vec.y);
	buf.push_back(vec.z);
}

void ProcTerrainScene::render()
{
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	Audace::ShaderProgram *shader = material->getShader();
	shader->bind();
	shader->setUniformVec4("ambientLight", 0.6, 0.6, 0.8, 1);
	shader->setUniformVec3("light[0].position", 30, 20, 30);
	shader->setUniformVec3("light[0].color", 0.8, 0.8, 0.5);
	shader->setUniformFloat("light[0].intensity", 1);
	shader->setUniformVec3("textureScale", 1.0, 1.0, 1.0);
	shader->setUniformVec3("viewPos", camera->getPosition());

	cubeSprite->render(this);
}

void ProcTerrainScene::disposeAssets()
{
}