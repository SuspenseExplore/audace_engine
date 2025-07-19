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
#include "content/JsonGui.h"
#include "renderer/Texture2d.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"
#include "renderer/material/Material.h"
#include "scene/BaseCamera.h"
#include "scene/SceneDescriptor.h"
#include "scene/graph/SceneGraph.h"
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

std::string guiPath = "ui/scene_editor.json";

SceneBuilder::SceneBuilder(Audace::BaseAppController* controller)
	: Scene(controller)
{
	strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
}

void SceneBuilder::loadAssets(Audace::IFileAccess* fileLoader)
{
	renderType = RenderType::FULL;
	this->fileLoader = fileLoader;

	modelIndex = fileLoader->textFileToJson("models/_index.json");
	shader = Audace::AssetStore::getShader("pbr");

	Audace::GltfLoader loader;
	loader.setImageLoadPath("images/_test/");
	loader.loadFile(fileLoader, "models/_test/", "Lantern.gltf");
	sceneGraph = loader.getSceneGraph(this);
	// sceneGraph = new Audace::SceneGraph(this);
	editor = new Audace::SceneEditor(fileLoader);
	editor->attachToScene(this);
	editor->setSceneGraph(sceneGraph);
}

void SceneBuilder::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ImGui::Begin("Gui Loader", nullptr, ImGuiWindowFlags_NoDecoration);
	// if (ImGui::Button("Reload GUI"))
	// {
	// 	delete jsonGui;
	// 	json jgui = fileLoader->textFileToJson(guiPath);
	// 	jsonGui = new Audace::JsonGui(jgui);
	// }
	// ImGui::End();
	// jsonGui->render();

	camera->update();
	sceneGraph->update(this);

	shader->bind();
	shader->setUniformVec3("viewPos", camera->getPosition());
	shader->setUniformVec4("ambientLight", ambientColor);

	for (auto& item : lights)
	{
		shader->setUniformLight(lightTypes[item.first], item.second);
	}

	// shader->setUniformFloat("outPosition", renderType == RenderType::POSITION ? 1.0 : 0.0);
	// shader->setUniformFloat("outMtlColor", renderType == RenderType::MTL_COLOR ? 1.0 : 0.0);
	// shader->setUniformFloat("outNormal", renderType == RenderType::NORMAL ? 1.0 : 0.0);
	// shader->setUniformFloat("outAmbient", renderType == RenderType::AMBIENT ? 1.0 : 0.0);
	// shader->setUniformFloat("outDirLight", renderType == RenderType::DIR_LIGHT ? 1.0 : 0.0);
	// shader->setUniformFloat("outFull", renderType == RenderType::FULL ? 1.0 : 0.0);

	for (Audace::Sprite* s : sprites)
	{
		s->renderWorldSpace(this);
	}
	editor->renderWorldSpace(this);
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

void SceneBuilder::setLight(int id, Audace::Sprite* light, std::string type)
{
	lights[id] = light;
	lightTypes[id] = type;
}

Audace::Sprite* SceneBuilder::getLight(int id)
{
	return lights[id];
}

void SceneBuilder::setCamera(Audace::BaseCamera* camera)
{
	this->camera = camera;
}

Audace::BaseCamera* SceneBuilder::getCamera()
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

#ifdef UNDEFINED_THING
	ImGui::Begin("Editor");
	ImGui::SetWindowPos(ImVec2(600, 800), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_Once);
	if (ImGui::BeginTabBar("Tabs1"))
	{
		if (ImGui::BeginTabItem("Render Output"))
		{
			if (ImGui::RadioButton("Position", renderType == RenderType::POSITION))
			{
				renderType = RenderType::POSITION;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Material", renderType == RenderType::MTL_COLOR))
			{
				renderType = RenderType::MTL_COLOR;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Normal", renderType == RenderType::NORMAL))
			{
				renderType = RenderType::NORMAL;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Ambient", renderType == RenderType::AMBIENT))
			{
				renderType = RenderType::AMBIENT;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Dir Light", renderType == RenderType::DIR_LIGHT))
			{
				renderType = RenderType::DIR_LIGHT;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Full", renderType == RenderType::FULL))
			{
				renderType = RenderType::FULL;
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
#endif
}

void SceneBuilder::disposeAssets()
{
}