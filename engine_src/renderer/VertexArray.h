#ifndef AU_VERTEXARRAY_H
#define AU_VERTEXARRAY_H

#include <vector>
#include "au_renderer.h"
#include "VertexAttribute.h"

namespace Audace {
	class VertexArray {
		GLuint glid;
		std::vector<VertexAttribute*> attrs;

		public:
		VertexArray(std::vector<VertexAttribute*> attributes) : attrs(attributes) {}
		
		void create();
		void bind();
		void destroy();
	};
}

#endif