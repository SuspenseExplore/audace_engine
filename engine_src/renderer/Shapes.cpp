//
// Created by Spencer on 5/22/2023.
//

#include "au_renderer.h"
#include "Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Audace {
	float* Shapes::squarePositions() {
		return new float[12]{
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f};
	}

	float* Shapes::circlePositions() {
		glm::vec4 center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 edge = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
		const int STEPS = 20;
		int n = 3;
		float *verts = new float[(STEPS + 2) * 3];
		verts[0] = center.x;
		verts[1] = center.y;
		verts[2] = center.z;
		for (int i = 0; i <= STEPS + 1; i++) {
			glm::mat4 mat = glm::rotate(glm::mat4(1.0f), (float) i * glm::two_pi<float>() / (float) STEPS, axis);
			glm::vec3 p = mat * edge;
			AU_RENDERER_LOG_DEBUG("{},{},{}", p.x, p.y, p.z);
			verts[n + 0] = p.x;
			verts[n + 1] = p.y;
			verts[n + 2] = p.z;
			n += 3;
		}
		return verts;
	}
}