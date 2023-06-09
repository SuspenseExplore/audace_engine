//
// Created by Spencer on 5/22/2023.
//

#include "Sprite.h"
#include "DataBuffer.h"
#include "VertexAttribute.h"
#include "VertexArray.h"

namespace Audace
{
	Sprite::Sprite(Model *model)
	{
		int vertexCount = model->vertices.size();
		int indexCount = model->indices.size();
		DataBuffer *buf = new DataBuffer(model->vertices.data(), vertexCount * 9 * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		DataBuffer *ind = new DataBuffer(model->indices.data(), indexCount * sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		buf->create();
		ind->create();

		VertexAttribute posAttr(0, 3, GL_FLOAT, false, sizeof(float) * 9, 0);
		VertexAttribute normAttr(1, 3, GL_FLOAT, false, sizeof(float) * 9, sizeof(float) * 3);
		VertexAttribute texCoordAttr(2, 3, GL_FLOAT, false, sizeof(float) * 9, sizeof(float) * 6);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(&posAttr);
		attrs.push_back(&normAttr);
		attrs.push_back(&texCoordAttr);
		VertexArray *vertArray = new VertexArray(attrs);
		vertArray->create();

		for (ModelSection* section : model->sections) {
			Mesh* mesh = new Mesh(vertArray, ind, section->startIndex, section->vertexCount, GL_TRIANGLES, GL_UNSIGNED_INT);
			meshes.push_back(mesh);
		}
	}

	void Sprite::render()
	{
		for (Mesh *mesh : meshes)
		{
			mesh->render();
		}
	}
} // Audace