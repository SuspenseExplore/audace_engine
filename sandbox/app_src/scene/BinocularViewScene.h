#ifndef AU_BINOCULARVIEWSCENE_H
#define AU_BINOCULARVIEWSCENE_H

#include <vector>
#include <map>
#include <string>
#include "scene/Scene.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class SceneEditor;
	class SceneGraph;
	class SceneGraphNode;
	class JsonGui;
	class ShaderProgram;
	class Sprite;
	class TextureCubemap;
	struct TypedLight;
	class FrameBuffer;
	class IFileAccess;
}

class BinocularViewScene : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 1);
	Audace::IFileAccess *fileLoader;
	std::string sceneFilepath;

	Audace::SceneEditor *editor;
	Audace::SceneGraph *sceneGraph = nullptr;
	Audace::JsonGui *jsonGui;

	static const int scenePathLength = 64;
	char sceneWritePath[scenePathLength];
	json jsonContent = {};
	json modelIndex;
	std::vector<std::string> indexPath = {};

	Audace::BaseCamera *camera;

	Audace::ShaderProgram *shader;
	glm::vec4 ambientColor = {1.0, 1.0, 1.0, 0.03};
	std::map<std::string, Audace::TypedLight *> lights;
	glm::mat4 modelMat;

	Audace::TextureCubemap *cubeTex;
	Audace::TextureCubemap *cubeConvTex;
	Audace::SceneGraphNode *skyboxNode;
	Audace::FrameBuffer *frameBuffer[2];

	int renderType = 0;

public:
	BinocularViewScene(Audace::BaseAppController *controller);

	void setClearColor(glm::vec4 color) override;
	void setAmbientLight(glm::vec4 color) override;
	glm::vec4 getAmbientLight() override { return ambientColor; }
	void setLight(Audace::LightType type, Audace::Sprite *sprite) override;
	Audace::TypedLight *getLight(const std::string &name) override;

	void loadModel(std::string path, std::string filename);
	void traverseModelIndex(json index, int i);

	void loadAssets(Audace::IFileAccess *fileLoader) override;
	void render() override;
	void renderUi() override;
	void disposeAssets() override;
	void reloadScene() override;

	void setCamera(Audace::BaseCamera *camera) override;
	Audace::BaseCamera *getCamera() override;
	void teleport(glm::vec3 pos) override;
};

#endif