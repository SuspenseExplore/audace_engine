#include "VertexAttribute.h"

namespace Audace {
	void VertexAttribute::enable() {
		glEnableVertexAttribArray(index);
	}

	void VertexAttribute::disable() {
		glEnableVertexAttribArray(index);
	}

	void VertexAttribute::bind() {
		enable();
		glVertexAttribPointer(index, size, type, normalized, stride, (void*) offset);
	}
}