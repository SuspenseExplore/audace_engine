#include "Mesh.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/ShaderProgram.h"
#include "renderer/material/BaseMaterial.h"

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
			glDrawElements(renderMode, vertexCount, indexType, (void *)byteOffset);
		}
	}
}