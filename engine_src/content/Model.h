#ifndef AU_MODEL_H
#define AU_MODEL_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "renderer/Material.h"

namespace Audace {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCoord;
	};

	struct ModelSection {
		int startIndex;
		int vertexCount;
		Material* material;
	};

	struct Model {
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<ModelSection*> sections;
	};
}

#endif