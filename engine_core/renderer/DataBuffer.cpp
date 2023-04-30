#include "au_platform.h"
#include "au_renderer.h"
#include "DataBuffer.h"

void Audace::DataBuffer::create()
{
	CALL_GL(glGenBuffers, 1, &glid);
	bind();
	CALL_GL(glBufferData, target, size, data, usage);
}

void Audace::DataBuffer::bind()
{
	CALL_GL(glBindBuffer, target, glid);
}

void Audace::DataBuffer::unbind()
{
	CALL_GL(glBindBuffer, target, 0);
}
