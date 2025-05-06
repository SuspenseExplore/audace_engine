#ifndef AU_VOXELTERRAINGEN_H
#define AU_VOXELTERRAINGEN_H

#include "renderer/Mesh.h"
#include "FastNoiseLite.h"
#include <future>
#include <chrono>

namespace Audace
{
	class VoxelTerrainGen
	{
		FastNoiseLite noise;

	public:
		static struct ChunkBuilder
		{
			unsigned int chunkSize;

			FastNoiseLite &noise;
			glm::vec3 id;
			std::vector<float> verts;
			std::vector<unsigned int> inds;
			std::vector<glm::vec3> positions;
			bool ready = false;
			bool loaded = false;
			std::future<bool> future;

			ChunkBuilder(glm::vec3 id, FastNoiseLite &noise, unsigned int chunkSize) : id(id), noise(noise), chunkSize(chunkSize)
			{
			}
			bool genPositions();
			void addToBuffer(std::vector<float> &buf, glm::vec3 vec);
			Audace::Mesh *makeMesh();

			std::string idString()
			{
				return "(" + std::to_string((int)id.x) + "," + std::to_string((int) id.y) + "," + std::to_string((int) id.z) + ")";
			}

			static std::string idString(glm::vec3 &id)
			{
				return "(" + std::to_string((int)id.x) + "," + std::to_string((int) id.y) + "," + std::to_string((int) id.z) + ")";
			}

			bool readyToLoad()
			{
				if (ready && !loaded)
				{
					return true;
				}
				if (!loaded && future.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
				{
					ready = true;
					loaded = true;
					return true;
				}
				return false;
			}
		};

		VoxelTerrainGen();
		ChunkBuilder* builder(glm::vec3 id, unsigned int chunkSize) { return new ChunkBuilder(id, noise, chunkSize); }
	};
};

#endif