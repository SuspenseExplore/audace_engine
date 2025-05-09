//
// Created by Spencer on 5/22/2023.
//

#include "glm/gtc/matrix_transform.hpp"
#include "Sprite.h"
#include "DataBuffer.h"
#include "VertexAttribute.h"
#include "VertexArray.h"

namespace Audace
{
	Sprite::Sprite(Model *model)
	{
		modelMatrix = glm::mat4(1.0f);
		pose.position = glm::vec3(0.0f);
		pose.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f);

		int vertexCount = model->vertices.size();
		DataBuffer *buf = new DataBuffer(model->vertices.data(), vertexCount * 12 * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buf->create();

		VertexAttribute posAttr(0, 3, GL_FLOAT, false, sizeof(float) * 12, 0);
		VertexAttribute texCoordAttr(1, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 3);
		VertexAttribute normalAttr(2, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 6);
		VertexAttribute tangentAttr(3, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 9);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(&posAttr);
		attrs.push_back(&texCoordAttr);
		attrs.push_back(&normalAttr);
		attrs.push_back(&tangentAttr);
		vertexArray = new VertexArray(attrs);
		vertexArray->create();

		for (ModelSection *section : model->sections)
		{
			int indexCount = section->indices.size();
			DataBuffer *ind = new DataBuffer(section->indices.data(), indexCount * sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
			ind->create();

			Mesh *mesh = new Mesh(vertexArray, ind, 0, indexCount, GL_TRIANGLES, GL_UNSIGNED_INT, section->material);
			meshes.push_back(mesh);
		}
	}

	Sprite::Sprite(std::vector<Mesh *> meshes)
	{
		modelMatrix = glm::mat4(1.0f);
		pose.position = glm::vec3(0.0f);
		pose.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f);
		this->meshes = meshes;
	}

	void Sprite::render(Scene *scene)
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), pose.position);
		worldMat *= glm::mat4_cast(pose.orientation);
		worldMat = glm::scale(worldMat, scale);
		worldMat *= modelMatrix;
		for (Mesh *mesh : meshes)
		{
			mesh->getMaterial()->getShader()->bind();
			mesh->getMaterial()->getShader()->setUniformMat4("vpMat", scene->getCamera()->getViewProjMatrix());
			mesh->render(worldMat);
		}
	}
} // Audace