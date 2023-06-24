#ifndef AU_POINTLIGHT_H
#define AU_POINTLIGHT_H

#include "glm/glm.hpp"

namespace Audace {
	struct PointLight {
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
	};
}

#endif