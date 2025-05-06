//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SHAPES_H
#define AU_SHAPES_H

#include "renderer/Mesh.h"
#include "renderer/DataBuffer.h"

namespace Audace {
	class Shapes {
	public:
		static Mesh* squarePositions();
		static Mesh* circlePositions();
		static Mesh* cubePositions();
		static Mesh* cubePosNormTan();
		static DataBuffer* cubeBuffer();
		static Mesh* spherePositions(int xResolution, int yResolution);
	};
}

#endif //AU_SHAPES_H
