#ifndef AU_DATABUFFER_H
#define AU_DATABUFFER_H

#include "au_renderer.h"

namespace Audace
{
	class DataBuffer
	{
		GLuint glid = -1;
		void *data;
		GLuint size;
		GLuint target;
		GLuint usage;

	public:
		DataBuffer(void *data, GLuint size, GLuint bufferTarget, GLuint usageHint) : data(data), size(size), target(bufferTarget), usage(usageHint) {}
		~DataBuffer() {destroy();}
		void create();
		void bind();
		void unbind();
		void destroy()
		{
			if (glid > 0)
			{
				glDeleteBuffers(1, &glid);
				AU_CHECK_GL_ERRORS();
				AU_RENDERER_LOG_TRACE("Destroyed DataBuffer {}", glid);
				glid = -1;
			}
		}
	};
}

#endif