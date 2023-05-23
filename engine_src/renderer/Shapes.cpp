//
// Created by Spencer on 5/22/2023.
//

#include <vector>
#include "au_renderer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Audace {
	Sprite* Shapes::squarePositions() {
		float *verts = new float[]{
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f};
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * 12, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		VertexAttribute *attr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute*> attrs;
		attrs.push_back(attr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Sprite *sprite = new Sprite(vertexBuffer, nullptr, vertexArray, 0, 4, GL_TRIANGLE_STRIP);
		return sprite;
	}

	Sprite* Shapes::circlePositions() {
		glm::vec4 center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 edge = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
		const int STEPS = 20;
		const int COUNT = (STEPS + 2) * 3;
		int n = 3;
		float *verts = new float[COUNT];
		verts[0] = center.x;
		verts[1] = center.y;
		verts[2] = center.z;
		for (int i = 0; i <= STEPS; i++) {
			glm::mat4 mat = glm::rotate(glm::mat4(1.0f), (float) i * glm::two_pi<float>() / (float) STEPS, axis);
			glm::vec3 p = mat * edge;
			AU_RENDERER_LOG_DEBUG("{},{},{}", p.x, p.y, p.z);
			verts[n + 0] = p.x;
			verts[n + 1] = p.y;
			verts[n + 2] = p.z;
			n += 3;
		}
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * COUNT, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		VertexAttribute *attr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute*> attrs;
		attrs.push_back(attr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Sprite *sprite = new Sprite(vertexBuffer, nullptr, vertexArray, 0, STEPS + 2, GL_TRIANGLE_FAN);
		return sprite;
	}
}