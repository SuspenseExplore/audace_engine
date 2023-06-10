#include "au_renderer.h"
#include "MainScene.h"
#include "ImageData.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Sprite.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

void MainScene::loadAssets()
{
	glClearColor(0, 0, 1, 0);
	boxSprite = Audace::Shapes::cubePosNorm();

	std::string vs = fileLoader->textFileToString("shaders/standard/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/standard/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();

	{
		Audace::ImageData img = fileLoader->readImageFile("images/dark_grid.png");
		darkGridTex = new Audace::Texture2d(img);
		darkGridTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/green_checkerboard.png");
		greenChecksTex = new Audace::Texture2d(img);
		greenChecksTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/orange_checkerboard.png");
		orangeChecksTex = new Audace::Texture2d(img);
		orangeChecksTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/purple_checkerboard.png");
		purpleChecksTex = new Audace::Texture2d(img);
		purpleChecksTex->create();
	}

	Audace::Model* model = fileLoader->readModelFile("models/", "wallNarrowWood.obj");
	for (Audace::ModelSection* section : model->sections) {
		section->material->setShader(shaderProgram);
	}
	modelSprite = new Audace::Sprite(model);
}

void MainScene::render()
{
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	darkGridTex->bind(1);
	greenChecksTex->bind(2);
	orangeChecksTex->bind(3);
	purpleChecksTex->bind(4);

	camera.setOrientation(cameraPitch, 0, cameraYaw);
	camera.move(cameraVel);

	shaderProgram->bind();
	shaderProgram->setUniformVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
	shaderProgram->setUniformVec3("light.ambient", 0.2f, 0.2f, 0.4f);
	shaderProgram->setUniformVec3("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	shaderProgram->setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram->setUniformVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	shaderProgram->setUniformMat4("vpMat", camera.getvpMat());

	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, -2.0f));
		worldMat = glm::scale(worldMat, glm::vec3(20.0f, 20.0f, 1.0f));
		shaderProgram->setUniformMat4("worldMat", worldMat);
		shaderProgram->setUniformInt("tex1", 1);
		shaderProgram->setUniformVec2("textureScale", 10, 10);
		boxSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0, 0));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		worldMat = glm::scale(worldMat, glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram->setUniformMat4("worldMat", worldMat);
		shaderProgram->setUniformInt("tex1", 2);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		modelSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		worldMat = glm::scale(worldMat, glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram->setUniformMat4("worldMat", worldMat);
		shaderProgram->setUniformInt("tex1", 3);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		modelSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 5));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		worldMat = glm::scale(worldMat, glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram->setUniformMat4("worldMat", worldMat);
		shaderProgram->setUniformInt("tex1", 4);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		modelSprite->render();
	}
}

void MainScene::disposeAssets()
{
	shaderProgram->destroy();
	delete shaderProgram;
}