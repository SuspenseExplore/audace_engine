#include "VertexArray.h"

namespace Audace {
	void VertexArray::create() {
		glGenVertexArrays(1, &glid);
		bind();
		for (VertexAttribute *attr : attrs) {
			attr->bind();
		}
		AU_RENDERER_LOG_TRACE("Create VertexArray {}", glid);
	}

	void VertexArray::bind() {
		glBindVertexArray(glid);
		AU_RENDERER_LOG_TRACE("Bound VertexArray {}", glid);
	}

	void VertexArray::destroy() {
		glDeleteVertexArrays(1, &glid);
		for (int i = 0; i < attrs.size(); i++) {
			delete attrs[i];
		}
		AU_RENDERER_LOG_TRACE("Destroyed VertexArray {}", glid);
		glid = -1;
	}
}