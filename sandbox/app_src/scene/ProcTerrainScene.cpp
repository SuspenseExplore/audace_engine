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
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

void ProcTerrainScene::loadAssets(Audace::FileLoader *fileLoader)
{
	shader = Audace::AssetStore::getShader("terrain");
	material = new Audace::Material;
	material->setName("groundMat");
	material->setShader(shader);
	material->setAmbientColor({1.0, 1.0, 1.0});
	material->setDiffuseColor({1.0, 1.0, 1.0});
	material->setSpecularColor({0.5, 0.5, 0.5});
	material->setEmissionColor({0.0, 0.0, 0.0});
	material->setAmbientOcclusionMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_AmbientOcclusion.jpg"));
	material->setDiffuseMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Color.jpg"));
	material->setNormalMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_NormalGL.jpg"));
	material->setSpecularMap(Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Roughness.jpg"));

	cubeSprite = Audace::AssetStore::getCubeSprite();
	cubeSprite->getMesh()->setMaterial(material);
}

void ProcTerrainScene::render()
{
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	glm::vec3 camPos = camera->getOriginPos() + camera->getPosition();
	glm::vec3 baseChunk = glm::vec3{(int)camPos.x / CHUNK_SIZE, (int)camPos.y / CHUNK_SIZE, (int)camPos.z / CHUNK_SIZE};

	// check for chunks that have loaded
	for (auto iter = loadingChunks.begin(); iter != loadingChunks.end(); iter++)
	{
		if (iter->second->readyToLoad())
		{
		}
	}

	// generate new chunks
	int rad = 2;
	for (int x = -rad; x < rad; x++)
	{
		for (int y = -rad; y < rad; y++)
		{
			for (int z = -rad; z < rad; z++)
			{
				glm::vec3 chunk = baseChunk + glm::vec3{x, y, z};
				std::string k = Audace::VoxelTerrainGen::ChunkBuilder::idString(chunk);
				if (loadingChunks.find(k) == loadingChunks.end())
				{
					Audace::VoxelTerrainGen::ChunkBuilder *cb = terrainGen.builder(chunk, CHUNK_SIZE);
					cb->future = std::async(std::launch::async | std::launch::deferred, &Audace::VoxelTerrainGen::ChunkBuilder::genPositions, cb);
					loadingChunks[k] = cb;
				}
			}
		}
	}

	Audace::ShaderProgram *shader = material->getShader();
	shader->bind();
	shader->setUniformVec4("ambientLight", 0.6, 0.6, 0.8, 1);
	shader->setUniformVec3("light[0].position", lightPos);
	shader->setUniformVec3("light[0].color", 0.8, 0.8, 0.5);
	shader->setUniformFloat("light[0].intensity", 1);
	shader->setUniformVec3("textureScale", 1.0, 1.0, 1.0);
	shader->setUniformVec3("viewPos", camPos);
	material->apply();

	for (auto iter = loadingChunks.begin(); iter != loadingChunks.end(); iter++)
	{
		Audace::VoxelTerrainGen::ChunkBuilder *cb = iter->second;
		if (cb->loaded && cb->positions.size() > 0)
		{
			shader->setUniformVec3Array("voxelPos[0]", glm::value_ptr(cb->positions[0]), cb->positions.size());
			glm::mat4 worldMat = glm::mat4(1.0);
			shader->setUniformMat4("worldMat", worldMat);
			shader->setUniformMat4("vpMat", camera->getViewProjMatrix());
			cubeSprite->getMesh()->getVertexArray()->bind();
			cubeSprite->getMesh()->getIndexBuffer()->bind();
			glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0, cb->positions.size());
		}
	}
}

void ProcTerrainScene::disposeAssets()
{
}