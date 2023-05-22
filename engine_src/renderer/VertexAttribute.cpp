#include "VertexAttribute.h"

namespace Audace {
	void VertexAttribute::enable() {
		glEnableVertexAttribArray(index);
		AU_CHECK_GL_ERRORS();
	}

	void VertexAttribute::disable() {
		glEnableVertexAttribArray(index);
		AU_CHECK_GL_ERRORS();
	}

	void VertexAttribute::bind() {
		enable();
		glVertexAttribPointer(index, size, type, normalized, stride, (void*) offset);
		AU_CHECK_GL_ERRORS();
	}
}