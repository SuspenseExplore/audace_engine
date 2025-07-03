#ifndef AU_GLTFVIEWERSCENE_H
#define AU_GLTFVIEWERSCENE_H

#include "scene/Scene.h"

namespace Audace
{
	class BaseAppController;
	class BaseCamera;
	class SceneGraph;
	class SceneGraphNode;
	class RotationAnimation;
	class PointLight;
}

class GltfViewerScene : public Audace::Scene {
	glm::vec4 clearColor = glm::vec4(0.25, 0.25, 0.25, 1);
	Audace::IFileAccess* fileLoader;
	Audace::BaseCamera* camera;

	Audace::SceneGraph* sceneGraph;
	Audace::PointLight* ptLight;
	Audace::RotationAnimation* anim;
	Audace::SceneGraphNode* lightNode;
public:
	GltfViewerScene(Audace::BaseAppController* controller);

	void loadAssets(Audace::IFileAccess* fileLoader) override;

	void render() override;

	void renderUi() override;

	void disposeAssets() override {}

	Audace::BaseCamera* getCamera() override;

	void setCamera(Audace::BaseCamera* camera) override;

	void teleport(glm::vec3 pos) override;
};
#endif