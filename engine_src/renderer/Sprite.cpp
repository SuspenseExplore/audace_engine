//
// Created by Spencer on 5/22/2023.
//

#include "Sprite.h"

namespace Audace {
	void Sprite::render() {
		if (indexBuffer == nullptr) {
			vertexArray->bind();
			glDrawArrays(renderMode, startIndex, vertexCount);
		} else {
			vertexArray->bind();
			indexBuffer->bind();
			glDrawElements(renderMode, vertexCount, indexType, (void*) startIndex);
		}
	}
} // Audace