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
		DataBuffer *buf = new DataBuffer(model->vertices.data(), vertexCount * 9 * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buf->create();

		VertexAttribute posAttr(0, 3, GL_FLOAT, false, sizeof(float) * 9, 0);
		VertexAttribute normAttr(1, 3, GL_FLOAT, false, sizeof(float) * 9, sizeof(float) * 3);
		VertexAttribute texCoordAttr(2, 3, GL_FLOAT, false, sizeof(float) * 9, sizeof(float) * 6);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(&posAttr);
		attrs.push_back(&normAttr);
		attrs.push_back(&texCoordAttr);
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

	void Sprite::render()
	{
		for (Mesh *mesh : meshes)
		{
			mesh->render();
		}
	}
} // Audace