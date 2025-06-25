#include "VertexAttribute.h"

namespace Audace {
	VertexAttribute::VertexAttribute(std::string attrName, std::string typeName, GLenum type, bool normalized, GLuint stride, GLuint offset) :
		type(type), normalized(normalized), stride(stride), offset(offset)
	{
		if (attrName == "POSITION")
		{
			index = 0;
		}
		else if (attrName == "NORMAL")
		{
			index = 2;
		}
		else if (attrName == "TEXCOORD_0")
		{
			index = 1;
		}
		else if (attrName == "TEXCOORD_1")
		{
			index = 4;
		}

		if (typeName == "SCALAR")
		{
			size = 1;
		}
		else if (typeName == "VEC2")
		{
			size = 2;
		}
		else if (typeName == "VEC3")
		{
			size = 3;
		}
		else if (typeName == "VEC4")
		{
			size = 4;
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
		if (buffer != nullptr)
		{
			buffer->bind(GL_ARRAY_BUFFER);
		}
		enable();
		glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
		AU_CHECK_GL_ERRORS();
	}
}