#ifndef AU_VOXELTERRAINGEN_H
#define AU_VOXELTERRAINGEN_H

#include "renderer/Mesh.h"
#include "FastNoiseLite.h"

namespace Audace
{
	class VoxelTerrainGen
	{
		FastNoiseLite noise;
		unsigned int chunkSize = 32;

		public:
		VoxelTerrainGen();

		Mesh* genChunk(glm::vec3 chunkId);
		void addToBuffer(std::vector<float> &buf, glm::vec3 vec);
	};
};

#endif