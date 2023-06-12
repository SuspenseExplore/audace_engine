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
		Material* material;
		std::vector<unsigned int> indices;
	};

	struct Model {
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<ModelSection*> sections;
	};
}

#endif