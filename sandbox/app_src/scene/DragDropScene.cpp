#include <sstream>
#include "au_renderer.h"
#include "DragDropScene.h"
#include "ImageData.h"
#include "content/AssetStore.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Sprite.h"
#include "imgui.h"
#include "SceneEnum.h"

void DragDropScene::loadAssets()
{
	glClearColor(0, 0, 1, 0);

	shaderProgram = Audace::AssetStore::simpleBillboardShader();

	{
		int i = 0;
		std::stringstream ss;
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 13; k++)
			{
				ss.str("");
				ss << "images/playing_cards/card" << suitNames[j] << rankNames[k] << ".png";
				Audace::ImageData img = fileLoader->readImageFile(ss.str());
				Audace::Texture2d *tex = new Audace::Texture2d(img);
				tex->create();
				cardTextures[i++] = tex;
			}
		}
	}
	material = new Audace::SimpleBillboardMaterial;
	material->setShader(shaderProgram);
	material->setColor(glm::vec4(1.0f, 1.0, 1.0f, 1.0f));

	Audace::Mesh *mesh = Audace::Shapes::cubePositions();
	mesh->setMaterial((Audace::BaseMaterial*) material);
	sprite = new Audace::Sprite(std::vector<Audace::Mesh *>{mesh});
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(140.0f, 190.0f, 1.0f));
	sprite->setModelMatrix(modelMat);
	sprite->setPosition(glm::vec3(0, 0, 0));
}

Audace::Sprite *DragDropScene::loadSprite(std::string filename)
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

void DragDropScene::render()
{
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	material->setTexture(cardTextures[0]);

	shaderProgram->bind();
	shaderProgram->setUniformMat4("vpMat", camera.getViewProjMatrix());
	sprite->render();

	ImGui::Begin("Scenes");
	if (ImGui::Button("Navigation"))
	{
		appController->setScene(SandboxScene::NAVIGATION);
	}
	ImGui::End();
}

void DragDropScene::disposeAssets()
{
	shaderProgram->destroy();
	delete shaderProgram;
}