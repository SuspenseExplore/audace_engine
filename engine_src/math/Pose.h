//
// Created by Suspense on 5/30/2023.
//

#ifndef AU_POSE_H
#define AU_POSE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Audace {
	struct Pose {
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		Pose() {}
		Pose(glm::vec3 pos, glm::quat ori) : position(pos), orientation(ori) {}
	};
}

#endif //AU_POSE_H
