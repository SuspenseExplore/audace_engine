#ifndef AI_SCENEBUILDER_H
#define AI_SCENEBUILDER_H

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
	class JsonGui;
	class ShaderProgram;
	class Sprite;
	class PointLight;
	class DirLight;
	class IFileAccess;
}

class SceneBuilder : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 1);
	Audace::IFileAccess* fileLoader;
	Audace::SceneEditor* editor;
	Audace::SceneGraph* sceneGraph;
	Audace::JsonGui* jsonGui;

	static const int scenePathLength = 64;
	char sceneWritePath[scenePathLength];
	json jsonContent = {};
	json modelIndex;
	std::vector<std::string> indexPath = {};

	Audace::BaseCamera* camera;

	Audace::ShaderProgram* shader;
	glm::vec4 ambientColor = { 1.0, 1.0, 1.0, 0.03 };
	std::map<int, Audace::Sprite*> lights;
	std::map<int, std::string> lightTypes;
	glm::mat4 modelMat;

	int renderType = 0;

public:
	SceneBuilder(Audace::BaseAppController* controller);

	void setClearColor(glm::vec4 color) override;
	void setAmbientLight(glm::vec4 color) override;
	glm::vec4 getAmbientLight() override { return ambientColor; }
	void setLight(int id, Audace::Sprite* light, std::string type) override;
	Audace::Sprite* getLight(int id) override;

	void loadModel(std::string path, std::string filename);
	void traverseModelIndex(json index, int i);

	void loadAssets(Audace::IFileAccess* fileLoader) override;
	void render() override;
	void renderUi() override;
	void disposeAssets() override;
	void setCamera(Audace::BaseCamera* camera) override;
	Audace::BaseCamera* getCamera() override;
	void teleport(glm::vec3 pos) override;
};

#endif