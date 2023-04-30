#ifndef AU_VERTEXARRAY_H
#define AU_VERTEXARRAY_H

#include "au_platform.h"
#include "au_renderer.h"

namespace Audace {
	class VertexArray {
		GLuint glid;

		public:
		~VertexArray() {
			destroy();
		}

		void create();
		void destroy() {
			if (glid > 0) {
				CALL_GL(glDeleteVertexArrays, 1, &glid);
				glid = -1;
			}
		}

		void bind() {CALL_GL(glBindVertexArray, glid);}
		void unbind() {CALL_GL(glBindVertexArray, 0);}
	};
}

#endif