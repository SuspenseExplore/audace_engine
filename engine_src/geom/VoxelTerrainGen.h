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
			unsigned int chunkSize = 32;

			FastNoiseLite &noise;
			glm::vec3 id;
			std::vector<float> verts;
			std::vector<unsigned int> inds;
			bool ready = false;
			bool loaded = false;
			std::future<bool> future;

			ChunkBuilder(glm::vec3 id, FastNoiseLite &noise) : id(id), noise(noise)
			{
			}
			bool genGeometry();
			void addToBuffer(std::vector<float> &buf, glm::vec3 vec);
			Audace::Mesh *makeMesh();

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
					return true;
				}
				return false;
			}
		};

		VoxelTerrainGen();
		ChunkBuilder* builder(glm::vec3 id) { return new ChunkBuilder(id, noise); }
	};
};

#endif