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

	pointLight = new Audace::PointLight();
	pointLight->setPosition({0, 0, 0});
	pointLight->setColor({1, 1, 1});
	pointLight->setIntensity(1);
	addSprite(pointLight);

	Audace::AssetStore::getWhiteTexture()->bind(0);
	shader->setUniformInt("material.diffuseMap", 0);

	editWin = new Audace::SpriteEditWindow();
}

void SceneBuilder::loadModel(std::string path, std::string filename)
{
	currSprite = Audace::AssetStore::cloneSprite(path + filename);
	currSprite->forEachMesh([this](Audace::Mesh *mesh)
							{ mesh->getMaterial()->setShader(shader); });
	currSprite->setModelMatrix(modelMat);
	currSprite->setName(filename + "_" + std::to_string(nextSpriteId++));

	builderSprites.push_back(currSprite);
	editWin->setSprite(currSprite);
}

void SceneBuilder::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shader->bind();
	shader->setUniformVec4("ambientLight", ambientColor);
	shader->setUniformVec3("light[0].position", pointLight->getPosition());
	shader->setUniformVec3("light[0].color", pointLight->getColor());
	shader->setUniformFloat("light[0].intensity", pointLight->getIntensity());

	for (Audace::Sprite *s : sprites)
	{
		s->renderWorldSpace(this);
	}
	for (Audace::Sprite *s : builderSprites)
	{
		s->renderWorldSpace(this);
	}

	if (currSprite != nullptr)
	{
		currSprite->renderWorldSpace(this);
		editWin->renderWorldSpace(this);
	}
}

void SceneBuilder::renderUi()
{
	if (currSprite != nullptr)
	{
		editWin->renderViewSpace(this);
	}

	ImGui::Begin("Editor");
	ImGui::SetWindowPos(ImVec2(600, 800), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(600, 600), ImGuiCond_Once);
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

		if (ImGui::BeginTabItem("Sprites"))
		{
			if (ImGui::BeginListBox("Sprites"))
			{
				for (Audace::Sprite *s : builderSprites)
				{
					bool selected = (currSprite != nullptr && s->getName() == currSprite->getName());
					if (ImGui::Selectable(s->getName().c_str(), selected))
					{
						currSprite = s;
						editWin->setSprite(s);
						if (selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndListBox();
			}
			if (ImGui::Button("Remove"))
			{
				for (auto iter = builderSprites.begin(); iter != builderSprites.end(); iter++)
				{
					if ((*iter)->getName() == currSprite->getName())
					{
						builderSprites.erase(iter);
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