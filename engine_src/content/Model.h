#ifndef AU_MODEL_H
#define AU_MODEL_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "renderer/material/BaseMaterial.h"

namespace Audace {
	struct Vertex {
		glm::vec3 position = {0, 0, 0};
		glm::vec3 texCoord = {0, 0, 0};
		glm::vec3 normal = {0, 0, 0};
		glm::vec3 tangent = {0, 0, 0};
	};

	struct ModelSection {
		BaseMaterial* material;
		std::vector<unsigned int> indices;
	};

	struct Model {
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<ModelSection*> sections;
	};
}

#endif