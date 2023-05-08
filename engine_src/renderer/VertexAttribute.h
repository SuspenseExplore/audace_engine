#ifndef AU_VERTEXATTRIBUTE_H
#define AU_VERTEXATTRIBUTE_H

#include "au_renderer.h"

namespace Audace {
	class VertexAttribute {
		GLuint index;
		GLuint size;
		GLenum type;
		GLboolean normalized;
		GLuint stride;
		GLuint offset;

		public:
		VertexAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, GLuint offset) :
			index(index), size(size), type(type), normalized(normalized), stride(stride), offset(offset) {}

		void enable();
		void disable();
		void bind();
	};
}

#endif