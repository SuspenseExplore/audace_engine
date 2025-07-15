//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SHAPES_H
#define AU_SHAPES_H

namespace Audace {
	class Mesh;
	class DataBuffer;

	class Shapes {
	public:
		static Mesh* squarePositions();
		static Mesh* circlePositions();
		static Mesh* cubePositions();
		static Mesh* cubePosNormTan();
		static Mesh* cubeWireframe();
		static DataBuffer* cubeBuffer();
		static Mesh* spherePositions(int xResolution, int yResolution);
		static Mesh* coloredAxes();
	};
}

#endif //AU_SHAPES_H
