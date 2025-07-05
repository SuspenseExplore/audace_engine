#ifndef AI_SCENEBUILDER_H
#define AI_SCENEBUILDER_H

#include <vector>
#include <map>
#include "scene/Scene.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class SceneEditor;
	class SceneGraph;
	class ShaderProgram;
	class PointLight;
	class IFileAccess;
}

class SceneBuilder : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 1);
	Audace::IFileAccess* fileLoader;
	Audace::SceneEditor* editor;
	Audace::SceneGraph* sceneGraph;

	static const int scenePathLength = 64;
	char sceneWritePath[scenePathLength];
	json jsonContent = {};
	json modelIndex;
	std::vector<std::string> indexPath = {};

	Audace::BaseCamera* camera;

	Audace::ShaderProgram* shader;
	glm::vec4 ambientColor = { 0.5, 0.5, 0.5, 1.0 };
	glm::vec3 dirLightDirection = { 0, 0, -1 };
	glm::vec4 dirLightColor = { 1, 1, 0.8, 0.7 };
	std::vector<Audace::PointLight*> ptLights;
	glm::mat4 modelMat;

	int renderType = 0;

public:
	SceneBuilder(Audace::BaseAppController* controller);

	void setClearColor(glm::vec4 color) override;
	void setAmbientLight(glm::vec4 color) override;
	void setDirLight(glm::vec3 dir, glm::vec4 color) override;
	void setPointLight(int i, glm::vec3 pos, glm::vec4 color) override;
	void setPointLight(int i, Audace::PointLight* p) override;

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