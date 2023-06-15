#ifndef AU_MESH_H
#define AU_MESH_H

#include "au_renderer.h"
#include "glm/glm.hpp"
#include "renderer/VertexArray.h"
#include "renderer/DataBuffer.h"
#include "renderer/Material.h"

namespace Audace
{
	/**
	 * Encapsulates the vertex array and meta data for rendering a single object
	 */
	class Mesh
	{
		VertexArray *vertexArray;
		DataBuffer *indexBuffer;
		int startIndex;
		int vertexCount;
		GLenum renderMode;
		GLenum indexType;
		Material *material;

	public:
		Mesh(VertexArray *vertexArray, DataBuffer *indexBuffer, int startIndex, int vertexCount, GLenum renderMode, GLenum indexType, Material *material) : vertexArray(vertexArray), indexBuffer(indexBuffer), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode), indexType(indexType), material(material) {}
		Mesh(VertexArray *vertexArray, int startIndex, int vertexCount, GLenum renderMode, Material *material) : vertexArray(vertexArray), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode), material(material) {}

		void render(glm::mat4 worldMat = glm::mat4(1.0f));
		Mesh* clone() {
			return new Mesh(vertexArray, indexBuffer, startIndex, vertexCount, renderMode, indexType, material);
		}
	};
}

#endif