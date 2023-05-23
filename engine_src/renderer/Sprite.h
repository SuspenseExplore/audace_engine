//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SPRITE_H
#define AU_SPRITE_H

#include "au_renderer.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

namespace Audace {

	/**
	 * A Sprite contains the geometry and metadata to render a single discrete object
	 */
	class Sprite {
		DataBuffer *vertexBuffer;
		DataBuffer *indexBuffer;
		VertexArray *vertexArray;
		int startIndex;
		int vertexCount;
		GLenum renderMode;

	public:
		Sprite(DataBuffer *vertexBuffer, DataBuffer *indexBuffer, VertexArray *vertexArray,
			   int startIndex, int vertexCount, GLenum renderMode) :
				vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), vertexArray(vertexArray),
				startIndex(startIndex), vertexCount(vertexCount), renderMode(renderMode) {}

		void render();
	};

} // Audace

#endif //AU_SPRITE_H
