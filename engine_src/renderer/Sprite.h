//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SPRITE_H
#define AU_SPRITE_H

#include <vector>
#include "au_renderer.h"
#include "glm/glm.hpp"
#include "math/Pose.h"
#include "content/Model.h"
#include "renderer/Mesh.h"
#include "renderer/VertexArray.h"

namespace Audace {

	/**
	 * Combines one or more Meshes with world transformation data to make a single coherent renderable object
	 */
	class Sprite {
		VertexArray* vertexArray;
		std::vector<Mesh*> meshes;
		glm::mat4 modelMatrix;
		Pose pose;
		glm::vec3 scale;

		Sprite(Sprite* sprite) {
			vertexArray = sprite->vertexArray;
			for (Mesh* mesh : sprite->meshes) {
				meshes.push_back(mesh);
			}
			modelMatrix = glm::mat4(sprite->modelMatrix);
			pose.position = glm::vec3(sprite->pose.position);
			pose.orientation = glm::quat(sprite->pose.orientation);
			scale = glm::vec3(sprite->scale);
		}

	public:
		Sprite(Model* model);

		void render();
		Sprite* clone() {
			return new Sprite(this);
		}

		void setModelMatrix(glm::mat4 mat) {
			modelMatrix = mat;
		}

		void setPosition(glm::vec3 position) {
			pose.position = position;
		}

		void setOrientation(glm::quat orientation) {
			pose.orientation = orientation;
		}

		void setScale(glm::vec3 scale) {
			this->scale = glm::vec3(scale);
		}
	};

} // Audace

#endif //AU_SPRITE_H
