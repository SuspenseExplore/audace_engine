#include "VoxelTerrainGen.h"

Audace::VoxelTerrainGen::VoxelTerrainGen()
{
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.01f);
}

void Audace::VoxelTerrainGen::ChunkBuilder::addToBuffer(std::vector<float> &buf, glm::vec3 vec)
{
	buf.push_back(vec.x);
	buf.push_back(vec.y);
	buf.push_back(vec.z);
}

bool Audace::VoxelTerrainGen::ChunkBuilder::genGeometry()
{
	unsigned int baseInd = 0;
	glm::vec3 basePos = (float)chunkSize * id;

	float noiseVals[32][32][32];

	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			for (int z = 0; z < chunkSize; z++)
			{
				// TODO: this needs to be actual world coords
				glm::vec3 pos = basePos + glm::vec3{x, y, z};
				noiseVals[x][y][z] = -pos.z + noise.GetNoise(pos.x, pos.y, pos.z) * 20;
			}
		}
	}

	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			for (int z = 0; z < chunkSize; z++)
			{
				float noiseVal = noiseVals[x][y][z];
				glm::vec3 pos = basePos + glm::vec3{x, y, z};
				if (noiseVal < 0)
				{
					continue;
				}
				if (x > 0 && x < chunkSize - 1 && 
					y > 0 && y < chunkSize - 1 && 
					z > 0 && z < chunkSize - 1 &&
					noiseVals[x-1][y][z] > 0 &&
					noiseVals[x+1][y][z] > 0 &&
					noiseVals[x][y-1][z] > 0 &&
					noiseVals[x][y+1][z] > 0 &&
					noiseVals[x][y][z-1] > 0 &&
					noiseVals[x][y][z+1] > 0)
				{
					continue;
				}

				// -x
				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {0, 0, 0});	// texCoord
				addToBuffer(verts, {-1, 0, 0}); // normal
				addToBuffer(verts, {0, 1, 0});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {0, 1, 0});	// texCoord
				addToBuffer(verts, {-1, 0, 0}); // normal
				addToBuffer(verts, {0, 1, 0});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {1, 0, 0});	// texCoord
				addToBuffer(verts, {-1, 0, 0}); // normal
				addToBuffer(verts, {0, 1, 0});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {1, 1, 0});	// texCoord
				addToBuffer(verts, {-1, 0, 0}); // normal
				addToBuffer(verts, {0, 1, 0});	// tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;

				// +x
				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {0, 0, 0}); // texCoord
				addToBuffer(verts, {1, 0, 0}); // normal
				addToBuffer(verts, {0, -1, 0}); // tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {0, 1, 0}); // texCoord
				addToBuffer(verts, {1, 0, 0}); // normal
				addToBuffer(verts, {0, -1, 0}); // tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {1, 0, 0}); // texCoord
				addToBuffer(verts, {1, 0, 0}); // normal
				addToBuffer(verts, {0, -1, 0}); // tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {1, 1, 0}); // texCoord
				addToBuffer(verts, {1, 0, 0}); // normal
				addToBuffer(verts, {0, -1, 0}); // tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;

				// -y
				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {0, 0, 0});	// texCoord
				addToBuffer(verts, {0, -1, 0}); // normal
				addToBuffer(verts, {0, 0, 1});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {0, 1, 0});	// texCoord
				addToBuffer(verts, {0, -1, 0}); // normal
				addToBuffer(verts, {0, 0, 1});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {1, 0, 0});	// texCoord
				addToBuffer(verts, {0, -1, 0}); // normal
				addToBuffer(verts, {0, 0, 1});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {1, 1, 0});	// texCoord
				addToBuffer(verts, {0, -1, 0}); // normal
				addToBuffer(verts, {0, 0, 1});	// tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;

				// +y
				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {0, 0, 0}); // texCoord
				addToBuffer(verts, {0, 1, 0}); // normal
				addToBuffer(verts, {0, 0, -1});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {0, 1, 0}); // texCoord
				addToBuffer(verts, {0, 1, 0}); // normal
				addToBuffer(verts, {0, 0, -1});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {1, 0, 0}); // texCoord
				addToBuffer(verts, {0, 1, 0}); // normal
				addToBuffer(verts, {0, 0, -1});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {1, 1, 0}); // texCoord
				addToBuffer(verts, {0, 1, 0}); // normal
				addToBuffer(verts, {0, 0, -1});	// tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;

				// -z
				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {0, 0, 0});	// texCoord
				addToBuffer(verts, {0, 0, -1}); // normal
				addToBuffer(verts, {1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {0, 1, 0});	// texCoord
				addToBuffer(verts, {0, 0, -1}); // normal
				addToBuffer(verts, {1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 0});
				addToBuffer(verts, {1, 0, 0});	// texCoord
				addToBuffer(verts, {0, 0, -1}); // normal
				addToBuffer(verts, {1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 0});
				addToBuffer(verts, {1, 1, 0});	// texCoord
				addToBuffer(verts, {0, 0, -1}); // normal
				addToBuffer(verts, {1, 0, 0});	// tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;

				// +z
				addToBuffer(verts, {pos.x + 0, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {0, 0, 0}); // texCoord
				addToBuffer(verts, {0, 0, 1}); // normal
				addToBuffer(verts, {-1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 0, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {0, 1, 0}); // texCoord
				addToBuffer(verts, {0, 0, 1}); // normal
				addToBuffer(verts, {-1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 0, pos.z + 1});
				addToBuffer(verts, {1, 0, 0}); // texCoord
				addToBuffer(verts, {0, 0, 1}); // normal
				addToBuffer(verts, {-1, 0, 0});	// tangent

				addToBuffer(verts, {pos.x + 1, pos.y + 1, pos.z + 1});
				addToBuffer(verts, {1, 1, 0}); // texCoord
				addToBuffer(verts, {0, 0, 1}); // normal
				addToBuffer(verts, {-1, 0, 0});	// tangent
				inds.push_back(baseInd);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 1);
				inds.push_back(baseInd + 2);
				inds.push_back(baseInd + 3);
				baseInd += 4;
			}
		}
	}
	return true;
}

Audace::Mesh* Audace::VoxelTerrainGen::ChunkBuilder::makeMesh()
{
	Audace::DataBuffer *vertexBuffer = new Audace::DataBuffer(verts.data(), sizeof(float) * verts.capacity(), GL_ARRAY_BUFFER,
															  GL_STATIC_DRAW);
	vertexBuffer->create();
	vertexBuffer->bind();

	Audace::DataBuffer *indexBuffer = new Audace::DataBuffer(inds.data(), sizeof(unsigned int) * inds.capacity(),
															 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	indexBuffer->create();

	Audace::VertexAttribute *posAttr = new Audace::VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 12, 0);
	Audace::VertexAttribute *texCoordAttr = new Audace::VertexAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 3);
	Audace::VertexAttribute *normAttr = new Audace::VertexAttribute(2, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 6);
	Audace::VertexAttribute *tanAttr = new Audace::VertexAttribute(3, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 9);
	std::vector<Audace::VertexAttribute *> attrs;
	attrs.push_back(posAttr);
	attrs.push_back(texCoordAttr);
	attrs.push_back(normAttr);
	attrs.push_back(tanAttr);
	Audace::VertexArray *vertexArray = new Audace::VertexArray(attrs);
	vertexArray->create();

	Audace::Mesh *mesh = new Audace::Mesh(vertexArray, indexBuffer, 0, inds.capacity(), GL_TRIANGLES,
										  GL_UNSIGNED_INT, nullptr);
	loaded = true;
	return mesh;
}