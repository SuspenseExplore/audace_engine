#include "DataBuffer.h"

void Audace::DataBuffer::create() {
	if (glid == -1) {
		glGenBuffers(1, &glid);
		AU_RENDERER_LOG_TRACE("Created DataBuffer with id {}", glid);
		bind();
		glBufferData(target, size, data, usage);
	}
}

void Audace::DataBuffer::bind() {
	glBindBuffer(target, glid);
	AU_CHECK_GL_ERRORS();
	AU_RENDERER_LOG_TRACE("Bound DataBuffer {}", glid);
}

void Audace::DataBuffer::unbind() {
	glBindBuffer(target, 0);
	AU_CHECK_GL_ERRORS();
}
