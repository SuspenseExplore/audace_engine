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
	Mesh *Shapes::squarePositions() {
		float *verts = new float[]{
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f};
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * 12, GL_ARRAY_BUFFER,
												  GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		VertexAttribute *attr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(attr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, 0, 4, GL_TRIANGLE_STRIP);
		return mesh;
	}

	Mesh *Shapes::circlePositions() {
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
			glm::mat4 mat = glm::rotate(glm::mat4(1.0f),
										(float) i * glm::two_pi<float>() / (float) STEPS, axis);
			glm::vec3 p = mat * edge;
			AU_RENDERER_LOG_DEBUG("{},{},{}", p.x, p.y, p.z);
			verts[n + 0] = p.x;
			verts[n + 1] = p.y;
			verts[n + 2] = p.z;
			n += 3;
		}
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * COUNT, GL_ARRAY_BUFFER,
												  GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		VertexAttribute *attr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(attr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, 0, STEPS + 2, GL_TRIANGLE_FAN);
		return mesh;
	}

	Mesh *Shapes::cubePositions() {
		float *verts = new float[]{
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f,
		};

		unsigned short *indices = new unsigned short[]{
				0, 1, 2,
				1, 2, 3,
				4, 5, 6,
				5, 6, 7,
				0, 1, 4,
				1, 4, 5,
				2, 3, 6,
				3, 6, 7,
				0, 2, 4,
				2, 4, 6,
				1, 3, 5,
				3, 5, 7
		};
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * 24, GL_ARRAY_BUFFER,
												  GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		DataBuffer *indexBuffer = new DataBuffer(indices, sizeof(unsigned short) * 36,
												 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		indexBuffer->create();

		VertexAttribute *attr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(attr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, indexBuffer, 0, 36, GL_TRIANGLES,
									GL_UNSIGNED_SHORT);
		return mesh;
	}

	Mesh *Shapes::cubePosNorm() {
		float *verts = new float[]{
				//-x
				0, 0, 0, -1, 0, 0,
				0, 0, 1, -1, 0, 0,
				0, 1, 0, -1, 0, 0,
				0, 1, 1, -1, 0, 0,
				//+x
				1, 0, 0,	1, 0, 0,
				1, 0, 1,	1, 0, 0,
				1, 1, 0,	1, 0, 0,
				1, 1, 1,	1, 0, 0,
				//-y
				0, 0, 0,	0, -1, 0,
				0, 0, 1,	0, -1, 0,
				1, 0, 0,	0, -1, 0,
				1, 0, 1,	0, -1, 0,
				//+y
				0, 1, 0,	0, 1, 0,
				0, 1, 1,	0, 1, 0,
				1, 1, 0,	0, 1, 0,
				1, 1, 1,	0, 1, 0,
				//-z
				0, 0, 0,		0, 0, -1,
				0, 1, 0,	0, 0, -1,
				1, 0, 0,	0, 0, -1,
				1, 1, 0,	0, 0, -1,
				//+z
				0, 0, 1,	0, 0, 1,
				0, 1, 1,	0, 0, 1,
				1, 0, 1,	0, 0, 1,
				1, 1, 1,	0, 0, 1
		};

		unsigned short *indices = new unsigned short[]{
				0, 1, 2,
				1, 2, 3,
				4, 5, 6,
				5, 6, 7,
				8, 9, 10,
				9, 10, 11,
				12, 13, 14,
				13, 14, 15,
				16, 17, 18,
				17, 18, 19,
				20, 21, 22,
				21, 22, 23
		};
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * 6 * 4 * 6, GL_ARRAY_BUFFER,
												  GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		DataBuffer *indexBuffer = new DataBuffer(indices, sizeof(unsigned short) * 36,
												 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		indexBuffer->create();

		VertexAttribute *posAttr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
		VertexAttribute *normAttr = new VertexAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 6, sizeof(float) * 3);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(posAttr);
		attrs.push_back(normAttr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, indexBuffer, 0, 36, GL_TRIANGLES,
									GL_UNSIGNED_SHORT);
		return mesh;
	}
}