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
#include "scene/SceneDescriptor.h"
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

void SceneBuilder::loadAssets(Audace::FileLoader* fileLoader)
{
	renderType = RenderType::FULL;
	this->fileLoader = fileLoader;
	modelMat = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(1.0, 0.0, 0.0));

	modelIndex = fileLoader->textFileToJson("models/_index.json");
	shader = Audace::AssetStore::getShader("obj_mtl");

	pointLight = new Audace::PointLight();
	pointLight->setPosition({ 0, 0, 10 });
	pointLight->setColor({ 1, 1, 1 });
	pointLight->setIntensity(1);
	addSprite(pointLight);

	Audace::AssetStore::getWhiteTexture()->bind(0);
	shader->setUniformInt("material.diffuseMap", 0);

	// loadModel("kenney/nature/cliffs/", "cliff_scene.obj");

	editor = new Audace::SceneEditor(fileLoader);
	editor->attachToScene(this);
	editor->load(fileLoader->assetReadBasePath() + "scenes/", "cliffs.json");
}

void SceneBuilder::loadModel(std::string path, std::string filename)
{
}

void SceneBuilder::render()
{
	editor->syncToScene();

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shader->bind();
	shader->setUniformVec4("ambientLight", ambientColor);
	shader->setUniformVec4("dirLightColor", dirLightColor);
	shader->setUniformVec3("dirLightDirection", dirLightDirection);
	shader->setUniformVec3("light[0].position", pointLight->getPosition());
	shader->setUniformVec3("light[0].color", pointLight->getColor());
	shader->setUniformFloat("light[0].intensity", pointLight->getIntensity());

	shader->setUniformFloat("outPosition", renderType == RenderType::POSITION ? 1.0 : 0.0);
	shader->setUniformFloat("outMtlColor", renderType == RenderType::MTL_COLOR ? 1.0 : 0.0);
	shader->setUniformFloat("outNormal", renderType == RenderType::NORMAL ? 1.0 : 0.0);
	shader->setUniformFloat("outAmbient", renderType == RenderType::AMBIENT ? 1.0 : 0.0);
	shader->setUniformFloat("outDirLight", renderType == RenderType::DIR_LIGHT ? 1.0 : 0.0);
	shader->setUniformFloat("outFull", renderType == RenderType::FULL ? 1.0 : 0.0);

	for (Audace::Sprite* s : sprites)
	{
		s->renderWorldSpace(this);
	}
	editor->renderWorldSpace(this);
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

		if (ImGui::BeginTabItem("Files"))
		{
			traverseModelIndex(modelIndex, 0);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Sprites"))
		{
			if (ImGui::BeginListBox("Sprites"))
			{
				for (int i = 0; i < sprites.size(); i++)
				{
					Audace::Sprite* s = sprites[i];

					bool selected = (currSprite != nullptr && currSprite == s);
					if (ImGui::Selectable((s->getName() + "_" + std::to_string(i)).c_str(), selected))
					{
						currSprite = s;
						editWin->setSprite(s);
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}
			if (ImGui::Button("Remove"))
			{
				for (auto iter = sprites.begin(); iter != sprites.end(); iter++)
				{
					if ((*iter)->getName() == currSprite->getName())
					{
						sprites.erase(iter);
						delete currSprite;
						currSprite = nullptr;
						break;
					}
				}
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Lighting"))
		{
			if (ImGui::BeginTabBar("Lighting"))
			{
				if (ImGui::BeginTabItem("Ambient light"))
				{
					ImGui::ColorPicker4("Ambient color", glm::value_ptr(ambientColor));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Directional Light"))
				{
					static glm::vec3 angles = { 0, 0, 0 };
					ImGui::DragFloat3("Direction", glm::value_ptr(angles));
					ImGui::ColorPicker4("Color", glm::value_ptr(dirLightColor));

					glm::mat4 m = glm::mat4(1.0);
					m = glm::rotate(m, glm::radians(angles.x), { 1, 0, 0 });
					m = glm::rotate(m, glm::radians(angles.y), { 0, 1, 0 });
					m = glm::rotate(m, glm::radians(angles.z), { 0, 0, 1 });
					dirLightDirection = m * glm::vec4(0, 1, 0, 0);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Point light"))
				{
					static glm::vec4 pointLightColor = glm::vec4(pointLight->getColor(), pointLight->getIntensity());
					static glm::vec3 lightPos = pointLight->getPosition();
					ImGui::DragFloat3("Position", glm::value_ptr(lightPos), 0.01);
					ImGui::ColorPicker4("Color", glm::value_ptr(pointLightColor));
					pointLight->setPosition(lightPos);
					pointLight->setColor(glm::vec3(pointLightColor));
					pointLight->setIntensity(pointLightColor.a);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}

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
	for (auto& el : folders.items())
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
		for (auto& el : files.items())
		{
			if (ImGui::Selectable(el.value().template get<std::string>().c_str(), false))
			{
				std::string path;
				for (std::string p : indexPath)
				{
					path += p;
					path += "/";
				}
				loadModel(path, el.value());
			}
		}
		ImGui::EndCombo();
	}
}

void SceneBuilder::disposeAssets()
{
}