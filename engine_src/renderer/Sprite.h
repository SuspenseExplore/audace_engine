//
// Created by Spencer on 5/22/2023.
//

#ifndef AU_SPRITE_H
#define AU_SPRITE_H

#include <vector>
#include <algorithm>
#include <functional>
#include "au_renderer.h"
#include "glm/glm.hpp"
#include "math/Pose.h"
#include "content/Model.h"
#include "renderer/Mesh.h"
#include "renderer/VertexArray.h"
#include "renderer/material/BaseMaterial.h"
#include "scene/Scene.h"

namespace Audace
{

	/**
	 * Combines one or more Meshes with world transformation data to make a single coherent renderable object
	 */
	class Sprite
	{
	protected:
		VertexArray *vertexArray;
		std::vector<Mesh *> meshes;
		glm::mat4 modelMatrix;
		Pose pose;
		glm::vec3 scale;

		Sprite(Sprite *sprite)
		{
			vertexArray = sprite->vertexArray;
			for (Mesh *mesh : sprite->meshes)
			{
				meshes.push_back(mesh);
			}
			modelMatrix = glm::mat4(sprite->modelMatrix);
			pose.position = glm::vec3(sprite->pose.position);
			pose.orientation = glm::quat(sprite->pose.orientation);
			scale = glm::vec3(sprite->scale);
		}

	public:
		Sprite(Model *model);
		Sprite(std::vector<Mesh *> meshes);

		virtual void render(Scene *scene);
		Sprite *clone()
		{
			return new Sprite(this);
		}

		void setModelMatrix(glm::mat4 mat)
		{
			modelMatrix = mat;
		}

		virtual void setPosition(glm::vec3 position)
		{
			pose.position = position;
		}

		virtual void setOrientation(glm::quat orientation)
		{
			pose.orientation = orientation;
		}

		virtual void setScale(glm::vec3 scale)
		{
			this->scale = glm::vec3(scale);
		}

		glm::vec3 getPosition() { return pose.position; }
		glm::quat getOrientation() { return pose.orientation; }
		glm::vec3 getScale() { return scale; }

		Mesh *getMesh(int i = 0) { return meshes[i]; }
		BaseMaterial *getMaterial(int i = 0) { return getMesh(i)->getMaterial(); }

		void forEachMesh(std::function<void(Mesh *)> fn)
		{
			std::for_each(meshes.begin(), meshes.end(), fn);
		}
	};

} // Audace

#endif // AU_SPRITE_H
