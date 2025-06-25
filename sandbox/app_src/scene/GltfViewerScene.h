#ifndef AU_GLTFVIEWERSCENE_H
#define AU_GLTFVIEWERSCENE_H

#include "scene/Scene.h"
#include "scene/graph/SceneGraph.h"
#include "renderer/light/PointLight.h"

class GltfViewerScene : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0.25, 0.25, 0.25, 1);
	Audace::IFileAccess* fileLoader;
	Audace::BaseCamera* camera;

	Audace::SceneGraph* sceneGraph;
	Audace::PointLight* ptLight;

public:
	GltfViewerScene(Audace::BaseAppController* controller) : Scene(controller) {}
	void loadAssets(Audace::IFileAccess* fileLoader) override;
	void render() override;
	void renderUi() override;
	void disposeAssets() override {}

	Audace::BaseCamera* getCamera() override { return camera; }
	void setCamera(Audace::BaseCamera* camera) override { this->camera = camera; }
};

#endif