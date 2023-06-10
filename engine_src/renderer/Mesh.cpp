#include "Mesh.h"

namespace Audace
{
	void Mesh::render()
	{
		if (material != nullptr) {
			material->apply();
		}
		if (indexBuffer == nullptr)
		{
			vertexArray->bind();
			glDrawArrays(renderMode, startIndex, vertexCount);
		}
		else
		{
			vertexArray->bind();
			indexBuffer->bind();
			glDrawElements(renderMode, vertexCount, indexType, (void *)startIndex);
		}
	}
}