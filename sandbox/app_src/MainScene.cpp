#include "au_renderer.h"
#include "MainScene.h"
#include "ImageData.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

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
}

void MainScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	darkGridTex->bind(1);
	greenChecksTex->bind(2);
	orangeChecksTex->bind(3);
	purpleChecksTex->bind(4);

	// make the light orbit a point over time
	float t = glfwGetTime() * 50.0f;
	float x = cos(glm::radians(t)) * 7.0f;
	float y = sin(glm::radians(t)) * 7.0f;

	cameraPos += cameraVel;
	cameraTarget += cameraVel;

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", 0.2f, 0.2f, 0.4f, 0.2f);
	shaderProgram->setUniformVec4("diffusePos", lightPos.x, lightPos.y, lightPos.z, 0);
	shaderProgram->setUniformVec4("diffuseColor", diffuseLight.x, diffuseLight.y, diffuseLight.z, diffuseLight.w);
	shaderProgram->setUniformVec4("viewPos", cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

	glm::mat4 viewMat = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 projMat = glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 100.0f);
	glm::mat4 vpMat = projMat * viewMat;
		shaderProgram->setUniformMat4("vpMat", glm::value_ptr(vpMat));

	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, -2.0f));
		worldMat = glm::scale(worldMat, glm::vec3(20.0f, 20.0f, 1.0f));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformInt("tex1", 1);
		shaderProgram->setUniformVec2("textureScale", 10, 10);
		boxSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0));
		// worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformInt("tex1", 2);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		boxSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));
		// worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformInt("tex1", 3);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		boxSprite->render();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 10));
		// worldMat = glm::rotate(worldMat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformInt("tex1", 4);
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		boxSprite->render();
	}
}

void MainScene::disposeAssets()
{
	shaderProgram->destroy();
	delete shaderProgram;
}