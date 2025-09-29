#include <fstream>
#include <sstream>
#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "content/IFileAccess.h"
#include "content/JsonSerializer.h"
#include "content/AssetStore.h"
#include "content/gltf/GltfLoader.h"
#include "content/gltf/GltfxReader.h"
#include "content/JsonGui.h"
#include "renderer/Texture2d.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"
#include "renderer/light/TypedLight.h"
#include "renderer/material/Material.h"
#include "scene/BaseCamera.h"
#include "scene/SceneDescriptor.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "util/StringUtil.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "editor/SceneEditor.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

enum RenderType
{
	POSITION,
	MTL_COLOR,
	NORMAL,
	AMBIENT,
	DIR_LIGHT,
	FULL
};

std::string guiPath = "ui/node_editor.json";

SceneBuilder::SceneBuilder(Audace::BaseAppController *controller)
	: Scene(controller)
{
	strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
}

void SceneBuilder::loadAssets(Audace::IFileAccess *fileLoader)
{
	renderType = RenderType::FULL;
	this->fileLoader = fileLoader;
	sceneFilepath = "scenes/samples/sample_scenes.gltfx";

	// modelIndex = fileLoader->textFileToJson("models/_index.json");

	setAmbientLight({1, 1, 1, 0.4});
	shader = Audace::AssetStore::getShader("pbr");

	editor = new Audace::SceneEditor(fileLoader);
	editor->attachToScene(this);
	reloadScene();
}

void SceneBuilder::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();
	sceneGraph->update(this);

	shader->bind();
	shader->setUniformVec3("viewPos", camera->getPosition());
	shader->setUniformVec4("ambientLight", ambientColor);

	for (auto &item : lights)
	{
		shader->setUniformLight(item.second);
	}

	editor->renderWorldSpace(this);
	for (Audace::Sprite *s : sprites)
	{
		s->renderWorldSpace(this);
	}
}

void SceneBuilder::reloadScene()
{
	if (sceneGraph != nullptr)
	{
		sceneGraph->dispose();
		delete sceneGraph;
	}
	Audace::GltfxReader reader(fileLoader);
	sceneGraph = new Audace::SceneGraph;
	Audace::SceneGraphNode *root = reader.readDefaultScene(sceneFilepath);
	sceneGraph->setRootNode(root);
	editor->setSceneGraph(sceneGraph);
}

void SceneBuilder::loadModel(std::string path, std::string filename)
{
}

void SceneBuilder::setClearColor(glm::vec4 color)
{
	clearColor = color;
}

void SceneBuilder::setAmbientLight(glm::vec4 color)
{
	ambientColor = color;
}

void SceneBuilder::setLight(Audace::LightType type, Audace::Sprite *sprite)
{
	const std::string &name = sprite->getName();
	switch (type)
	{
	case Audace::LightType::POINT_LIGHT:
	{
		Audace::PointLight *ptLight = reinterpret_cast<Audace::PointLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, ptLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->ptLight->setColor(ptLight->getColor());
			lights[name]->ptLight->setIntensity(ptLight->getIntensity());
		}
	}
	break;

	case Audace::LightType::DIRECTIONAL_LIGHT:
	{
		Audace::DirLight *dirLight = reinterpret_cast<Audace::DirLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, dirLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->dirLight->setColor(dirLight->getColor());
			lights[name]->dirLight->setIntensity(dirLight->getIntensity());
		}
	}
	break;

	case Audace::LightType::SPOTLIGHT:
	{
		Audace::SpotLight *spotLight = reinterpret_cast<Audace::SpotLight *>(sprite);
		if (lights.find(name) == lights.end())
		{
			// the light entry doesn't exist yet
			Audace::TypedLight *light = new Audace::TypedLight(name, spotLight);
			lights[name] = light;
		}
		else
		{
			lights[name]->spotLight->setColor(spotLight->getColor());
			lights[name]->spotLight->setIntensity(spotLight->getIntensity());
			lights[name]->spotLight->setInnerAngle(spotLight->getInnerAngle());
			lights[name]->spotLight->setOuterAngle(spotLight->getOuterAngle());
		}
	}
	break;
	}
}

Audace::TypedLight *SceneBuilder::getLight(const std::string &name)
{
	return lights[name];
}

void SceneBuilder::setCamera(Audace::BaseCamera *camera)
{
	this->camera = camera;
}

Audace::BaseCamera *SceneBuilder::getCamera()
{
	return camera;
}

void SceneBuilder::teleport(glm::vec3 pos)
{
	camera->setOriginPos(pos);
}

void SceneBuilder::renderUi()
{
	// if (currSprite != nullptr)
	// {
	// 	editWin->renderViewSpace(this);
	// }
	editor->sceneEditWindow();
}

void SceneBuilder::disposeAssets()
{
}