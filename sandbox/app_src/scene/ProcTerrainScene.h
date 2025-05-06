#ifndef AU_PROCTERRAINSCENE_H
#define AU_PROCTERRAINSCENE_H

#include "scene/Scene.h"
#include "renderer/material/Material.h"
#include "renderer/Sprite.h"
#include "geom/VoxelTerrainGen.h"
#include "glm/glm.hpp"
#include <map>
#include <string>
#include <vector>

class ProcTerrainScene : public Audace::Scene
{
	const int CHUNK_SIZE = 8;

	Audace::VoxelTerrainGen terrainGen;
	std::map<std::string, Audace::VoxelTerrainGen::ChunkBuilder*> loadingChunks;
	std::vector<Audace::Sprite*> sprites;

	Audace::Sprite *cubeSprite;
	Audace::ShaderProgram *shader;
	Audace::Material *material;

	glm::vec3 lightPos = glm::vec3{30, 20, 30};

	Audace::BaseCamera *camera;
	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	float cameraYaw = 0;
	float cameraPitch = 0;

	void generateTerrain();
	void addToBuffer(std::vector<float> &buf, glm::vec3 vec);

	public:
	ProcTerrainScene(Audace::BaseAppController *controller) : Scene(controller)
	{

	}

	void loadAssets(Audace::FileLoader *fileLoader) override;
	void render() override;
	void disposeAssets() override;

	void setCamera(Audace::BaseCamera *camera) override {this->camera = camera; }
	Audace::BaseCamera *getCamera() override { return camera; }

	void setLightPos(glm::vec3 pos) { lightPos = pos + camera->getOriginPos(); }
	void teleport() { camera->setOriginPos(lightPos - camera->getPosition()); }
};


#endif