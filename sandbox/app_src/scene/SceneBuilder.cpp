#include "au_renderer.h"
#include "SceneBuilder.h"
#include "imgui.h"
#include "SceneEnum.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "KeyboardManager.h"
#include "MouseManager.h"

void SceneBuilder::loadAssets()
{
	std::string vs = fileLoader->textFileToString("shaders/standard/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/standard/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();

	Audace::Model *model = fileLoader->readModelFile("models/", "cliff_blockSlope_rock.obj");
	for (Audace::ModelSection *section : model->sections)
	{
		section->material->setShader(shaderProgram);
	}
	currSprite = new Audace::Sprite(model);

	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camController.forwardAction);
	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camController.backwardAction);
	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camController.leftAction);
	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camController.rightAction);
	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camController.upAction);
	Audace::KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camController.downAction);
	Audace::MouseManager::addButtonChangedEventHandler(1, camController.rightMouseAction);
	Audace::MouseManager::setMouseMoveEventHandler(&camController.aimAction);
}

void SceneBuilder::render()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camController.update();

	shaderProgram->bind();
	shaderProgram->setUniformVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
	shaderProgram->setUniformVec3("light.ambient", 0.2f, 0.2f, 0.4f);
	shaderProgram->setUniformVec3("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	shaderProgram->setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram->setUniformVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	shaderProgram->setUniformMat4("vpMat", camera.getvpMat());

	currSprite->setPosition(spritePos);
	currSprite->setOrientation(glm::quat(glm::radians(spriteAngles)));
	currSprite->setScale(spriteScale);
	currSprite->render();

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

	ImGui::Begin("Scene Properties");
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(clearColor));
	ImGui::End();

	ImGui::Begin("Load Model");
	ImGui::End();

	ImGui::Begin("Sprite Properties");
	ImGui::DragFloat3("Position", glm::value_ptr(spritePos));
	ImGui::DragFloat3("Angles", glm::value_ptr(spriteAngles), 1, -180.0f, 180.0f);
	ImGui::DragFloat3("Scale", glm::value_ptr(spriteScale), 0.1f);
	ImGui::End();
}

void SceneBuilder::disposeAssets()
{
}