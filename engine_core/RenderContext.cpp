#include "au_platform.h"
#include "RenderContext.h"

namespace Audace {
	void RenderContext::render() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}