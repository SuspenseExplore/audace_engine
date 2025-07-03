#ifndef AU_VERTEXATTRIBUTE_H
#define AU_VERTEXATTRIBUTE_H

#include <string>
#include "au_renderer.h"

namespace Audace {
	class DataBuffer;

	class VertexAttribute {
		GLuint index;
		GLuint size;
		GLenum type;
		GLboolean normalized;
		GLuint stride;
		GLuint offset;

		DataBuffer* buffer = nullptr;

	public:
		VertexAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, GLuint offset) :
			index(index), size(size), type(type), normalized(normalized), stride(stride), offset(offset) {
		}
		VertexAttribute(std::string attrName, std::string typeName, GLenum type, bool normalized, GLuint stride, GLuint offset);

		void setBuffer(DataBuffer* buf) { buffer = buf; }
		void enable();
		void disable();
		void bind();
	};
}

#endif