#include "DataBuffer.h"

void Audace::DataBuffer::create()
{
	glGenBuffers(1, &glid);
	bind();
	glBufferData(target, size, data, usage);
	AU_RENDERER_LOG_TRACE("Created DataBuffer with id {}", glid);
}

void Audace::DataBuffer::bind()
{
	glBindBuffer(target, glid);
	AU_RENDERER_LOG_TRACE("Bound DataBuffer {}", glid);
}

void Audace::DataBuffer::unbind()
{
	glBindBuffer(target, 0);
}
