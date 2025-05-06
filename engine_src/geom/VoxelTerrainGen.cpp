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

bool Audace::VoxelTerrainGen::ChunkBuilder::genPositions()
{
	unsigned int baseInd = 0;
	glm::vec3 basePos = (float)chunkSize * id;

	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			for (int z = 0; z < chunkSize; z++)
			{
				// TODO: this needs to be actual world coords
				glm::vec3 pos = basePos + glm::vec3{x, y, z};
				float n = noise.GetNoise(pos.x, pos.y, pos.z) * 20;
				if (n > 0.0)
				{
					positions.push_back(pos);
				}
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