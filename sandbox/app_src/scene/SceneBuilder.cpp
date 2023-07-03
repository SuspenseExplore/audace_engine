#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
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
	{
		std::string vs = fileLoader->textFileToString("shaders/standard/vs.glsl");
		std::string fs = fileLoader->textFileToString("shaders/standard/fs.glsl");
		shaderProgram = new Audace::ShaderProgram(vs, fs);
		shaderProgram->create();
		shaderProgram->bind();
	}

	quadMesh = Audace::Shapes::squarePositions();
	font = new Audace::BitmapFont(fileLoader, "arial.ttf");

	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	modelNames[0] = "ground_grass";
	modelNames[1] = "ground_pathEnd";
	modelNames[2] = "ground_pathStraight";
	modelNames[3] = "ground_riverStraight";
	modelNames[4] = "bridge_stoneRoundNarrow";
	modelNames[5] = "cliff_scene";
	currModel = loadModel(modelNames[selectedModelIndex]);
	currSprite = new Audace::Sprite(currModel);
	currSprite->setModelMatrix(modelMat);

	pointLights[0] = Audace::PointLight{glm::vec3(0, 0, 5), glm::vec3(1, 0.7f, 0.2f), 1};
	pointLights[1] = Audace::PointLight{glm::vec3(-5, 0, 5), glm::vec3(1, 1, 0), 1};
	pointLights[2] = Audace::PointLight{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0};
	pointLights[3] = Audace::PointLight{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0};
}

Audace::Model *SceneBuilder::loadModel(std::string modelName)
{
	Audace::Model *model = fileLoader->readModelFile("models/", modelName + ".obj");
	for (Audace::ModelSection *section : model->sections)
	{
		section->material->setShader(shaderProgram);
	}
	return model;
}

void SceneBuilder::render()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.update();

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", 0.2f, 0.2f, 0.4f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		std::string prefix = "light[";
		prefix += '0' + i;
		prefix += "]";
		shaderProgram->setUniformVec3(prefix + ".position", glm::value_ptr(pointLights[i].position));
		shaderProgram->setUniformVec3(prefix + ".color", glm::value_ptr(pointLights[i].color));
		shaderProgram->setUniformFloat(prefix + ".intensity", pointLights[i].intensity);
	}
	// shaderProgram->setUniformVec3("light[1].position", glm::value_ptr(pointLights[1].position));
	// shaderProgram->setUniformVec3("light[1].color", glm::value_ptr(pointLights[1].color));
	// shaderProgram->setUniformFloat("light[1].intensity", pointLights[1].intensity);

	shaderProgram->setUniformVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	shaderProgram->setUniformMat4("vpMat", camera.getViewProjMatrix());

	for (Audace::Sprite *sprite : sprites)
	{
		sprite->render();
	}

	currSprite->setPosition(spritePos);
	currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
	currSprite->setScale(spriteScale);
	currSprite->render();

	// font->renderText("Sphynx of black quartz, judge my vow.");
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
			if (ImGui::BeginCombo("Filename", modelNames[selectedModelIndex].c_str()))
			{
				for (int i = 0; i < modelCount; i++)
				{
					bool selected = selectedModelIndex == i;
					if (ImGui::Selectable(modelNames[i].c_str(), selected))
					{
						selectedModelIndex = i;
						currModel = loadModel(modelNames[i]);
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