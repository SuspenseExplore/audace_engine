#ifndef AU_MODEL_H
#define AU_MODEL_H

#include <string>
#include <vector>
#include "glm/glm.hpp"

namespace Audace {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCoord;
	};

	struct Model {
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};
}

#endif