#include "DataBuffer.h"

void Audace::DataBuffer::create()
{
	glGenBuffers(1, &glid);
	bind();
	glBufferData(target, size, data, usage);
}

void Audace::DataBuffer::bind()
{
	glBindBuffer(target, glid);
}

void Audace::DataBuffer::unbind()
{
	glBindBuffer(target, 0);
}
