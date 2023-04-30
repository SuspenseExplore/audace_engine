#include "VertexArray.h"

namespace Audace {
	void VertexArray::create() {
		CALL_GL(glGenVertexArrays, 1, &glid);
	}
}