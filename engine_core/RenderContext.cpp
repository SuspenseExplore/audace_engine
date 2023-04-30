#include "au_platform.h"
#include "RenderContext.h"

void printGlErrors(char *fnName) {
	GLuint err = glGetError();
	while (err != GL_NO_ERROR) {
		AU_ENGINE_LOG_ERROR("GL error in {}: {}", fnName, err);
		err = glGetError();
	}
}

namespace Audace {
	void RenderContext::render() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}