#include "VertexAttribute.h"

namespace Audace {
	VertexAttribute::VertexAttribute(std::string attrName, std::string typeName, GLenum type, bool normalized, GLuint stride, GLuint offset) :
		type(type), normalized(normalized), stride(stride), offset(offset)
	{
		if (attrName == "POSITION")
		{
			index = 0;
		}

		if (typeName == "SCALAR")
		{
			size = 1;
		}
		else if (typeName == "VEC3")
		{
			size = 3;
		}
	}

	void VertexAttribute::enable() {
		glEnableVertexAttribArray(index);
		AU_CHECK_GL_ERRORS();
	}

	void VertexAttribute::disable() {
		glEnableVertexAttribArray(index);
		AU_CHECK_GL_ERRORS();
	}

	void VertexAttribute::bind() {
		enable();
		glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
		AU_CHECK_GL_ERRORS();
	}
}