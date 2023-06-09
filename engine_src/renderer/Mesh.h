#ifndef AU_MESH_H
#define AU_MESH_H

#include "au_renderer.h"
#include "renderer/VertexArray.h"
#include "renderer/DataBuffer.h"

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

	public:
		Mesh(VertexArray *vertexArray, DataBuffer *indexBuffer, int startIndex, int vertexCount, GLenum renderMode, GLenum indexType) : vertexArray(vertexArray), indexBuffer(indexBuffer), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode), indexType(indexType) {}
		Mesh(VertexArray *vertexArray, int startIndex, int vertexCount, GLenum renderMode) : vertexArray(vertexArray), startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode) {}

		void render();
	};
}

#endif