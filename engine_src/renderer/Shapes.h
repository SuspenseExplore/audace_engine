//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SHAPES_H
#define AU_SHAPES_H

#include "renderer/Sprite.h"

namespace Audace {
	class Shapes {
	public:
		static Sprite* squarePositions();
		static Sprite* circlePositions();
		static Sprite* cubePositions();
		static Sprite* cubePosNorm();
	};
}

#endif //AU_SHAPES_H
