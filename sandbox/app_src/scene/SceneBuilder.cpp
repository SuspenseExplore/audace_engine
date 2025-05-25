#include <fstream>
#include <sstream>
#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "content/JsonSerializer.h"
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Shapes.h"
#include "renderer/material/Material.h"
#include "util/StringUtil.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

void SceneBuilder::loadAssets(Audace::FileLoader *fileLoader)
{
	this->fileLoader = fileLoader;
	modelMat = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(1.0, 0.0, 0.0));

	modelIndex = fileLoader->textFileToJson("models/_index.json");
	shader = Audace::AssetStore::getShader("obj_mtl");
	Audace::Sprite *s = Audace::AssetStore::cloneSprite("kenney/nature/canoe.obj");

	s->forEachMesh([this](Audace::Mesh *mesh)
				   { mesh->getMaterial()->setShader(shader); });
	s->setModelMatrix(modelMat);
	sprites.push_back(s);

	pointLight = new Audace::PointLight();
	pointLight->setPosition({0, 0, 0});
	pointLight->setColor({1, 1, 1});
	pointLight->setIntensity(1);
}

void SceneBuilder::render()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shader->bind();
	shader->setUniformVec4("ambientLight", ambientColor);
	shader->setUniformVec3("light[0].position", pointLight->getPosition());
	shader->setUniformVec3("light[0].color", pointLight->getColor());
	shader->setUniformFloat("light[0].intensity", pointLight->getIntensity());

	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->renderWorldSpace(this);
	}
}

void SceneBuilder::renderUi()
{
	glClearColor(0, 0, 0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Begin("Editor");
	ImGui::SetWindowPos(ImVec2(400, 500));
	ImGui::SetWindowSize(ImVec2(800, 1000));
	if (ImGui::BeginTabBar("Tabs1"))
	{

		if (ImGui::BeginTabItem("Scene"))
		{
			ImGui::DragFloat4("Clear color", glm::value_ptr(clearColor), 0.01, 0.0, 1.0);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Files"))
		{
			traverseModelIndex(modelIndex, 0);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Lighting"))
		{
			ImGui::ColorPicker4("Ambient color", glm::value_ptr(ambientColor));
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void SceneBuilder::traverseModelIndex(json jsonListing, int level)
{
	json folders = jsonListing["folders"];
	json files = jsonListing["files"];

	// list the steps along the current path
	ImGui::NewLine();
	for (int i = 0; i < indexPath.size(); i++)
	{
		if (i == 0)
		{ // need a root node
			if (ImGui::Button(". >"))
			{
				indexPath.clear();
				return;
			}
		}
		std::string k = indexPath[i];
		ImGui::SameLine();
		if (i == indexPath.size() - 1)
		{
			ImGui::Text("%s >", k.c_str());
		}
		else if (ImGui::Button((k + " >").c_str()))
		{
			// if the button is clicked, switch to that level of the path
			std::vector<std::string> cpy = indexPath;
			indexPath.clear();
			int j = 0;
			std::string j1 = cpy[j];
			while (j < cpy.size() && j1.compare(k) != 0)
			{
				indexPath.push_back(j1);
				j++;
				j1 = cpy[j];
			}
			indexPath.push_back(k);
			return;
		}
		files = folders[k]["files"];
		folders = folders[k]["folders"];
	}

	// now list current contents; folder buttons, then files combo box
	for (auto &el : folders.items())
	{
		if (ImGui::Button(el.key().c_str()))
		{
			// add a new level to the path
			indexPath.push_back(el.key());
			return;
		}
	}

	if (ImGui::BeginCombo("Filename", ""))
	{
		for (auto &el : files.items())
		{
			if (ImGui::Selectable(el.value().template get<std::string>().c_str(), false))
			{
				AU_ENGINE_LOG_DEBUG("Selected {}", el.value());
			}
		}
		ImGui::EndCombo();
	}
	// int sel = 0;
	// std::string selValue = f1[sel].template get<std::string>();
	// if (ImGui::BeginCombo("Filename", selValue.c_str())) {
	// 	for (int i = 0; i < (int) f1.size(); i++) {
	// 		std::string v = f1[i].template get<std::string>();
	// 		if (ImGui::Selectable(v.c_str(), i == sel)) {
	// 			sel = i;
	// 		}
	// 		if (i == sel) {
	// 			ImGui::SetItemDefaultFocus();
	// 		}
	// 	}
	// 	ImGui::EndCombo();
	// }
}

void SceneBuilder::disposeAssets()
{
}