#ifndef AU_MESH_H
#define AU_MESH_H

#include "au_renderer.h"
#include "glm/glm.hpp"
#include "renderer/VertexArray.h"
#include "renderer/DataBuffer.h"
#include "renderer/material/BaseMaterial.h"

namespace Audace
{
	/**
	 * Encapsulates the vertex array and meta data for rendering a single object
	 */
	class Mesh
	{
		VertexArray *vertexArray;
		DataBuffer *indexBuffer = nullptr;
		int startIndex;
		int vertexCount;
		GLenum renderMode;
		GLenum indexType;
		BaseMaterial *material;

	public:
		Mesh(VertexArray *vertexArray, DataBuffer *indexBuffer, int startIndex, int vertexCount, GLenum renderMode, GLenum indexType, BaseMaterial *material)
			: vertexArray(vertexArray), indexBuffer(indexBuffer), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode), indexType(indexType), material(material) {}
		Mesh(VertexArray *vertexArray, int startIndex, int vertexCount, GLenum renderMode, BaseMaterial *material)
			: vertexArray(vertexArray), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode), material(material) {}

		void render(glm::mat4 worldMat = glm::mat4(1.0f));
		Mesh *clone()
		{
			return new Mesh(vertexArray, indexBuffer, startIndex, vertexCount, renderMode, indexType, material);
		}

		void setMaterial(BaseMaterial *mat) { material = mat; }
	};
}

#endif