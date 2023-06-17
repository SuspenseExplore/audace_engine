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
#include "imgui.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

void MainScene::loadAssets()
{
	glClearColor(0, 0, 1, 0);

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

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Sprite *cliff_slope_walls = loadSprite("cliff_blockSlopeWalls_rock.obj");
	Audace::Sprite *ground_grass = loadSprite("ground_grass.obj");
	Audace::Sprite *ground_path_straight = loadSprite("ground_pathStraight.obj");
	Audace::Sprite *ground_path_end = loadSprite("ground_pathEnd.obj");
	Audace::Sprite *river_straight = loadSprite("ground_riverStraight.obj");
	Audace::Sprite *bridge = loadSprite("bridge_stoneRoundNarrow.obj");

	{
		Audace::Sprite *sprite = ground_grass->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(-2, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_grass->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(-1, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_path_end->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(0, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_grass->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(1, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_grass->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(2, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_path_straight->clone();
		sprite->setModelMatrix(glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0)));
		sprite->setPosition(glm::vec3(0, 1, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_path_straight->clone();
		sprite->setModelMatrix(glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0)));
		sprite->setPosition(glm::vec3(0, 2, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = ground_path_end->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(0, 3, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = river_straight->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(-2, 4, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = river_straight->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(-1, 4, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = river_straight->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(0, 4, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = river_straight->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(1, 4, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = river_straight->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(2, 4, -1));
		sprites.push_back(sprite);
	}
	{
		Audace::Sprite *sprite = bridge->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(0, 4, -1));
		sprites.push_back(sprite);
	}
}

Audace::Sprite *MainScene::loadSprite(std::string filename)
{

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Model *model = fileLoader->readModelFile("models/", filename);
	for (Audace::ModelSection *section : model->sections)
	{
		section->material->setShader(shaderProgram);
	}
	Audace::Sprite *sprite = new Audace::Sprite(model);
	delete model;
	return sprite;
}

void MainScene::render()
{
	glDisable(GL_CULL_FACE);
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

	for (Audace::Sprite *sprite : sprites)
	{
		sprite->render();
	}

	bool checked;
	float f;
	ImGui::Begin("Test window");
	ImGui::SetWindowPos(ImVec2(20, 100));
	ImGui::Text("This is a text");
	ImGui::Checkbox("check me out", &checked);
	ImGui::SliderFloat("float", &f, 0, 1);
	ImGui::End();
}

void MainScene::disposeAssets()
{
	shaderProgram->destroy();
	delete shaderProgram;
}