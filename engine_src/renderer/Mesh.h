#ifndef AU_MESH_H
#define AU_MESH_H

#include "au_renderer.h"
#include "glm/glm.hpp"
#include <vector>

namespace Audace
{
	class VertexArray;
	class DataBuffer;
	class BaseMaterial;

	/**
	 * Encapsulates the vertex array and meta data for rendering a single object
	 */
	class Mesh
	{
		VertexArray* vertexArray;
		DataBuffer* indexBuffer = nullptr;
		int byteOffset;
		int vertexCount;
		GLenum renderMode;
		GLenum indexType;
		BaseMaterial* material;

	public:
		Mesh(VertexArray* vertexArray, DataBuffer* indexBuffer, int byteOffset, int vertexCount, GLenum renderMode, GLenum indexType, BaseMaterial* material)
			: vertexArray(vertexArray), indexBuffer(indexBuffer), byteOffset(byteOffset), vertexCount(vertexCount), renderMode(renderMode), indexType(indexType), material(material) {
		}
		Mesh(VertexArray* vertexArray, int byteOffset, int vertexCount, GLenum renderMode, BaseMaterial* material)
			: vertexArray(vertexArray), byteOffset(byteOffset), vertexCount(vertexCount), renderMode(renderMode), material(material) {
		}

		void render(glm::mat4 worldMat = glm::mat4(1.0f));
		void renderInstanced(std::vector<glm::mat4> mats);
		Mesh* clone()
		{
			return new Mesh(vertexArray, indexBuffer, byteOffset, vertexCount, renderMode, indexType, material);
		}

		VertexArray* getVertexArray() { return vertexArray; }
		DataBuffer* getIndexBuffer() { return indexBuffer; }
		void setMaterial(BaseMaterial* mat) { material = mat; }
		BaseMaterial* getMaterial() { return material; }
	};
}

#endif