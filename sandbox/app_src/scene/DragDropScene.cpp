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

	Audace::Mesh *mesh = Audace::Shapes::cubePositions();
	mesh->setMaterial((Audace::BaseMaterial *)material);
	sprite = new Audace::Sprite(std::vector<Audace::Mesh *>{mesh});
	sprite->setPosition(glm::vec3(0, 0, 0));
	sprite->setScale(glm::vec3(140.0f, 190.0f, 1.0f));
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

void DragDropScene::mouseMoved(float x, float y)
{
	glm::vec3 ray = glm::unProjectNO(glm::vec3(x, 720 - y, 0), camera.getViewMat(), camera.getProjMat(), glm::vec4(0, 0, 1280, 720));
	mousePos.x = ray.x;
	mousePos.y = ray.y;
	if (dragging)
	{
		sprite->setPosition(glm::vec3(mousePos.x - grabOffset.x, mousePos.y - grabOffset.y, 1.0f));
	}
	else if (isMouseOverCard(x, y))
	{
		material->setColor(glm::vec4(1.0f, 1.0f, 0.6f, 0.0f));
	}
	else
	{
		material->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void DragDropScene::buttonChanged(bool pressed)
{
	if (pressed && isMouseOverCard(mousePos.x, mousePos.y))
	{
		dragging = true;
		grabOffset = mousePos - glm::vec2(sprite->getPosition());
	}
	else if (dragging && !pressed)
	{
		dragging = false;
		sprite->setPosition(glm::vec3(mousePos.x - grabOffset.x, mousePos.y - grabOffset.y, 1.0f));
	}
}

bool DragDropScene::isMouseOverCard(float x, float y)
{
	glm::vec3 c1 = sprite->getPosition();
	glm::vec3 c2 = c1 + sprite->getScale();
	return !(mousePos.x < c1.x || mousePos.x > c2.x || mousePos.y < c1.y || mousePos.y > c2.y);
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