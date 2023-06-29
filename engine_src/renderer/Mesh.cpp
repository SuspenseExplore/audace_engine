#include "Mesh.h"

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
			glDrawArrays(renderMode, startIndex, vertexCount);
		}
		else
		{
			indexBuffer->bind();
			glDrawElements(renderMode, vertexCount, indexType, (void *)startIndex);
		}
	}
}