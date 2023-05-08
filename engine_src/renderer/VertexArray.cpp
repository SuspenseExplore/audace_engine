#include "VertexArray.h"

namespace Audace {
	void VertexArray::create() {
		glGenVertexArrays(1, &glid);
		bind();
		for (VertexAttribute *attr : attrs) {
			attr->bind();
		}
	}

	void VertexArray::bind() {
		glBindVertexArray(glid);
	}

	void VertexArray::destroy() {
		glDeleteVertexArrays(1, &glid);
		for (int i = 0; i < attrs.size(); i++) {
			delete attrs[i];
		}
	}
}