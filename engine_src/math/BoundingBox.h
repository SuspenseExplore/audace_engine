#ifndef AU_BOUNDINGBOX_H
#define AU_BOUNDINGBOX_H

#include "glm/glm.hpp"

namespace Audace
{
	struct BoundingBox
	{
		glm::vec3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
		glm::vec3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	};
}

#endif