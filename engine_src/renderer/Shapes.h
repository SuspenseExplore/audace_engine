//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SHAPES_H
#define AU_SHAPES_H

#include "renderer/Mesh.h"

namespace Audace {
	class Shapes {
	public:
		static Mesh* squarePositions();
		static Mesh* circlePositions();
		static Mesh* cubePositions();
		static Mesh* cubePosNorm();
	};
}

#endif //AU_SHAPES_H
