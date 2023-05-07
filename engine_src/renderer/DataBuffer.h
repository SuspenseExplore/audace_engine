#ifndef AU_DATABUFFER_H
#define AU_DATABUFFER_H

#include "au_renderer.h"

namespace Audace
{
	class DataBuffer
	{
		GLuint glid = 0;
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
				glid = -1;
			}
		}
	};
}

#endif