#include "Mesh.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/ShaderProgram.h"
#include "renderer/material/BaseMaterial.h"
#include "glm/gtc/type_ptr.hpp"

namespace Audace
{
	void Mesh::render(glm::mat4 worldMat)
	{
		vertexArray->bind();
		if (material != nullptr)
		{
			material->apply();
			material->getShader()->setUniformMat4("worldMat", worldMat);
		}
		if (indexBuffer == nullptr)
		{
			glDrawArrays(renderMode, byteOffset, vertexCount);
		}
		else
		{
			indexBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(renderMode, vertexCount, indexType, (void*)byteOffset);
		}
	}

	void Mesh::renderInstanced(std::vector<glm::mat4> mats)
	{
		int count = mats.size();
		vertexArray->bind();
		if (material != nullptr)
		{
			material->apply();
			material->getShader()->setUniformMat4Array("worldMat[0]", glm::value_ptr(mats[0]), count);
		}
		if (indexBuffer == nullptr)
		{
			glDrawArraysInstanced(renderMode, byteOffset, vertexCount, count);
		}
		else
		{
			indexBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElementsInstanced(renderMode, vertexCount, indexType, (void*)byteOffset, count);
		}
	}
}