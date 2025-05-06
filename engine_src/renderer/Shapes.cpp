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

		Mesh *mesh = new Mesh(vertexArray, 0, 4, GL_TRIANGLE_STRIP, nullptr);
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

		Mesh *mesh = new Mesh(vertexArray, 0, STEPS + 2, GL_TRIANGLE_FAN, nullptr);
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
									GL_UNSIGNED_SHORT, nullptr);
		return mesh;
	}
	// glm::vec3 dPos1 = verts[1]->position - verts[0]->position;
	// glm::vec3 dPos2 = verts[2]->position - verts[0]->position;
	// glm::vec3 dTexCoord1 = verts[1]->texCoord - verts[0]->texCoord;
	// glm::vec3 dTexCoord2 = verts[2]->texCoord - verts[0]->texCoord;
	// float r = 1.0f / (dTexCoord1.x * dTexCoord2.y - dTexCoord1.y * dTexCoord2.x);
	// glm::vec3 tangent = (dPos1 * dTexCoord2.y - dPos2 * dTexCoord1.y) * r;

	Mesh *Shapes::cubePosNormTan() {
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

		DataBuffer *vertexBuffer = cubeBuffer();
		DataBuffer *indexBuffer = new DataBuffer(indices, sizeof(unsigned short) * 36,
												 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		indexBuffer->create();

		VertexAttribute *posAttr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 12, 0);
		VertexAttribute *texCoordAttr = new VertexAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 3);
		VertexAttribute *normAttr = new VertexAttribute(2, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 6);
		VertexAttribute *tanAttr = new VertexAttribute(3, 3, GL_FLOAT, false, sizeof(float) * 12, sizeof(float) * 9);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(posAttr);
		attrs.push_back(texCoordAttr);
		attrs.push_back(normAttr);
		attrs.push_back(tanAttr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, indexBuffer, 0, 36, GL_TRIANGLES,
									GL_UNSIGNED_SHORT, nullptr);
		return mesh;
	}

	DataBuffer* Shapes::cubeBuffer()
	{
		float *verts = new float[]{
			//-x
			0, 0, 0,	0, 0, 0,	-1, 0, 0,	0, 1, 0,
			0, 0, 1,	0, 1, 0,	-1, 0, 0,	0, 1, 0,
			0, 1, 0,	1, 0, 0,	-1, 0, 0,	0, 1, 0,
			0, 1, 1,	1, 1, 0,	-1, 0, 0,	0, 1, 0,
			//+x	
			1, 0, 0,	0, 0, 0,	1, 0, 0,	0, -1, 0,
			1, 0, 1,	0, 1, 0,	1, 0, 0,	0, -1, 0,
			1, 1, 0,	1, 0, 0,	1, 0, 0,	0, -1, 0,
			1, 1, 1,	1, 1, 0,	1, 0, 0,	0, -1, 0,
			//-y	
			0, 0, 0,	0, 0, 0,	0, -1, 0,	0, 0, 1,
			0, 0, 1,	0, 1, 0,	0, -1, 0,	0, 0, 1,
			1, 0, 0,	1, 0, 0,	0, -1, 0,	0, 0, 1,
			1, 0, 1,	1, 1, 0,	0, -1, 0,	0, 0, 1,
			//+y	
			0, 1, 0,	0, 0, 0,	0, 1, 0,	0, 0, -1,
			0, 1, 1,	0, 1, 0,	0, 1, 0,	0, 0, -1,
			1, 1, 0,	1, 0, 0,	0, 1, 0,	0, 0, -1,
			1, 1, 1,	1, 1, 0,	0, 1, 0,	0, 0, -1,
			//-z	
			0, 0, 0,	0, 0, 0,	0, 0, -1,	1, 0, 0,
			0, 1, 0,	0, 1, 0,	0, 0, -1,	1, 0, 0,
			1, 0, 0,	1, 0, 0,	0, 0, -1,	1, 0, 0,
			1, 1, 0,	1, 1, 0,	0, 0, -1,	1, 0, 0,
			//+z	
			0, 0, 1,	0, 0, 0,	0, 0, 1,	-1, 0, 0,
			0, 1, 1,	0, 1, 0,	0, 0, 1,	-1, 0, 0,
			1, 0, 1,	1, 0, 0,	0, 0, 1,	-1, 0, 0,
			1, 1, 1,	1, 1, 0,	0, 0, 1,	-1, 0, 0,
		};
		DataBuffer *vertexBuffer = new DataBuffer(verts, sizeof(float) * 6 * 4 * 12, GL_ARRAY_BUFFER,
												GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();
		return vertexBuffer;
	}

	Mesh *Shapes::spherePositions(int xResolution, int yResolution) {
		const float pi = 3.14159265359f;
		std::vector<float> verts;
		std::vector<unsigned short> inds;
		
		for (int x = 0; x <= xResolution; ++x) {
			for (int y = 0; y <= yResolution; ++y) {
				float xSeg = (float)x / (float)xResolution;
				float ySeg = (float)y / (float)yResolution;
				verts.push_back(cos(xSeg * 2.0f * pi) * sin(ySeg * pi)); //x
				verts.push_back(cos(ySeg * pi)); //y
				verts.push_back(sin(xSeg * 2.0f * pi) * sin(ySeg * pi)); //z
			}
		}

		bool evenRow = true;
		for (int y = 0; y < yResolution; ++y) {
			if (evenRow) {
				for (int x = 0; x <= xResolution; ++x) {
					inds.push_back(y * (xResolution + 1) + x);
					inds.push_back((y + 1) * (xResolution + 1) + x);
				}
			} else {
				for (int x = xResolution; x >= 0; --x) {
					inds.push_back((y + 1) * (xResolution + 1) + x);
					inds.push_back(y * (xResolution + 1) + x);
				}
			}
			evenRow = !evenRow;
		}

		DataBuffer *vertexBuffer = new DataBuffer(&verts[0], sizeof(float) * verts.size(), GL_ARRAY_BUFFER,
												  GL_STATIC_DRAW);
		vertexBuffer->create();
		vertexBuffer->bind();

		DataBuffer *indexBuffer = new DataBuffer(&inds[0], sizeof(unsigned short) * inds.size(),
												 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		indexBuffer->create();

		VertexAttribute *posAttr = new VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
		std::vector<VertexAttribute *> attrs;
		attrs.push_back(posAttr);
		VertexArray *vertexArray = new VertexArray(attrs);
		vertexArray->create();

		Mesh *mesh = new Mesh(vertexArray, indexBuffer, 0, inds.size(), GL_TRIANGLE_STRIP,
									GL_UNSIGNED_SHORT, nullptr);
		return mesh;
	}
}