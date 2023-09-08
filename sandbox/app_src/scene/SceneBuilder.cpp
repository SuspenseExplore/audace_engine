#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Shapes.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef AU_PLATFORM_GLFW
#include "KeyboardManager.h"
#include "MouseManager.h"
#endif

void SceneBuilder::loadAssets()
{
	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("standard");
	shader->create();

	quadMesh = Audace::Shapes::squarePositions();

	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	modelBasePath = "models/";
	modelFiles = fileLoader->listFilesInDir(modelBasePath + "*.obj");
	currModel = loadModel(modelFiles[selectedModelIndex]);
	currSprite = new Audace::Sprite(currModel);
	currSprite->setModelMatrix(modelMat);

	pointLights[0] = Audace::PointLight{glm::vec3(0, 0, 5), glm::vec3(1, 0.7f, 0.2f), 1};
	pointLights[1] = Audace::PointLight{glm::vec3(-5, 0, 5), glm::vec3(1, 1, 0), 1};
	pointLights[2] = Audace::PointLight{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0};
	pointLights[3] = Audace::PointLight{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0};
}

Audace::Model *SceneBuilder::loadModel(std::string modelName)
{
	Audace::Model *model = fileLoader->readModelFile("models/", modelName);
	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("standard");
	for (Audace::ModelSection *section : model->sections)
	{
		section->material->setShader(shader);
	}
	return model;
}

void SceneBuilder::render()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.update();

	Audace::ShaderProgram *shader = Audace::AssetStore::getShader("standard");
	shader->bind();
	shader->setUniformVec4("ambientLight", 0.2f, 0.2f, 0.4f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		std::string prefix = "light[";
		prefix += '0' + i;
		prefix += "]";
		shader->setUniformVec3(prefix + ".position", glm::value_ptr(pointLights[i].position));
		shader->setUniformVec3(prefix + ".color", glm::value_ptr(pointLights[i].color));
		shader->setUniformFloat(prefix + ".intensity", pointLights[i].intensity);
	}
	// shader->setUniformVec3("light[1].position", glm::value_ptr(pointLights[1].position));
	// shader->setUniformVec3("light[1].color", glm::value_ptr(pointLights[1].color));
	// shader->setUniformFloat("light[1].intensity", pointLights[1].intensity);

	shader->setUniformVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	for (Audace::Sprite *sprite : sprites)
	{
		sprite->render(this);
	}

	currSprite->setPosition(spritePos);
	currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
	currSprite->setScale(spriteScale);
	currSprite->render(this);
	
	// ImGui::ShowDemoWindow();
	ImGui::Begin("Scenes");
	if (ImGui::Button("Navigation"))
	{
		appController->setScene(SandboxScene::NAVIGATION);
	}
	if (ImGui::Button("Main"))
	{
		appController->setScene(SandboxScene::MAIN);
	}
	ImGui::End();

	ImGui::Begin("Scene Editor");
	if (ImGui::BeginTabBar("Tools"))
	{
		if (ImGui::BeginTabItem("Clear State"))
		{
			ImGui::ColorEdit3("Clear Color", glm::value_ptr(clearColor));
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Models"))
		{
			if (ImGui::BeginCombo("Filename", modelFiles[selectedModelIndex].c_str()))
			{
				for (int i = 0; i < modelCount; i++)
				{
					bool selected = selectedModelIndex == i;
					if (ImGui::Selectable(modelFiles[i].c_str(), selected))
					{
						selectedModelIndex = i;
						currModel = loadModel(modelFiles[i]);
						currSprite = new Audace::Sprite(currModel);
						currSprite->setModelMatrix(modelMat);
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Add"))
			{
				currSprite->setPosition(spritePos);
				currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
				currSprite->setScale(spriteScale);
				sprites.push_back(currSprite);
				currSprite = new Audace::Sprite(currModel);
				currSprite->setModelMatrix(modelMat);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Sprite Properties"))
		{
			if (ImGui::BeginTable("World Tx", 4, 0));
			{
				ImGui::TableSetupColumn("Tx", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthStretch, 150);
				ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 150);
				ImGui::TableSetupColumn("z", ImGuiTableColumnFlags_WidthFixed, 150);
				ImGui::TableNextColumn();
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				ImGui::InputFloat("posx", &spritePos.x, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("posy", &spritePos.y, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("posz", &spritePos.z, 1.0f);

				ImGui::TableNextColumn();
				ImGui::Text("Angles");
				ImGui::TableNextColumn();
				ImGui::InputFloat("th-x", &spriteAngles.x, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("th-y", &spriteAngles.y, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("th-z", &spriteAngles.z, 1.0f);

				ImGui::TableNextColumn();
				ImGui::Text("Scale");
				ImGui::TableNextColumn();
				ImGui::InputFloat("scale x", &spriteScale.x, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("scale y", &spriteScale.y, 1.0f);
				ImGui::TableNextColumn();
				ImGui::InputFloat("scale z", &spriteScale.z, 1.0f);

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		static int currLight = 0;
		if (ImGui::BeginTabItem("Lights"))
		{
			ImGui::RadioButton("1", &currLight, 0);
			ImGui::SameLine();
			ImGui::RadioButton("2", &currLight, 1);
			ImGui::SameLine();
			ImGui::RadioButton("3", &currLight, 2);
			ImGui::SameLine();
			ImGui::RadioButton("4", &currLight, 3);

			ImGui::DragFloat3("Position", glm::value_ptr(pointLights[currLight].position));
			ImGui::ColorEdit3("Color", glm::value_ptr(pointLights[currLight].color));
			ImGui::SliderFloat("Intensity", &pointLights[currLight].intensity, 0.0f, 1.5f);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
	// ImGui::Begin("Lights");
	// if (ImGui::BeginTabBar("Light number", 0))
	// {
	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		std::string name = (std::string("Light ") + std::to_string(i));
	// 		if (ImGui::BeginTabItem(name.c_str()))
	// 		{
	// 			ImGui::DragFloat3((std::string("Position") + std::to_string(i)).c_str(), glm::value_ptr(pointLights[i].position));
	// 			ImGui::ColorEdit3((std::string("Color") + std::to_string(i)).c_str(), glm::value_ptr(pointLights[i].color));
	// 			ImGui::DragFloat((std::string("Intensity") + std::to_string(i)).c_str(), &pointLights[i].intensity, 0.1f, 0.0f, 1.5f);
	// 			ImGui::EndTabItem();
	// 		}
	// 	}
	// 	ImGui::EndTabBar();
	// }
	// ImGui::End();
}

void SceneBuilder::disposeAssets()
{
}