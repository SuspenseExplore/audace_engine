#include "VertexArray.h"

namespace Audace {
	void VertexArray::create() {
		glGenVertexArrays(1, &glid);
		AU_CHECK_GL_ERRORS();
		bind();
		for (VertexAttribute *attr : attrs) {
			attr->bind();
		}
		AU_RENDERER_LOG_TRACE("Create VertexArray {}", glid);
	}

	void VertexArray::bind() {
		glBindVertexArray(glid);
		AU_CHECK_GL_ERRORS();
		AU_RENDERER_LOG_TRACE("Bound VertexArray {}", glid);
	}

	void VertexArray::destroy() {
		glDeleteVertexArrays(1, &glid);
		AU_CHECK_GL_ERRORS();
		for (int i = 0; i < attrs.size(); i++) {
			delete attrs[i];
		}
		AU_RENDERER_LOG_TRACE("Destroyed VertexArray {}", glid);
		glid = -1;
	}
}