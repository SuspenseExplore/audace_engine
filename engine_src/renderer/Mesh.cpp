#include "Mesh.h"

namespace Audace
{
	void Mesh::render()
	{
		vertexArray->bind();
		if (material != nullptr) {
			material->apply();
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