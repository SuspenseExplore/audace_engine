#ifndef AU_PROCTERRAINSCENE_H
#define AU_PROCTERRAINSCENE_H

#include "scene/Scene.h"
#include "renderer/material/Material.h"
#include "renderer/Sprite.h"
#include "geom/VoxelTerrainGen.h"
#include "glm/glm.hpp"

class ProcTerrainScene : public Audace::Scene
{
	Audace::VoxelTerrainGen terrainGen;

	Audace::Sprite *cubeSprite;
	Audace::Material *material;

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
};


#endif