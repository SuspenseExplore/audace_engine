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

namespace Audace
{
	class Model;
	class Mesh;
	class BaseMaterial;
	class Scene;

	/**
	 * Combines one or more Meshes with world transformation data to make a single coherent renderable object
	 */
	class Sprite
	{
	protected:
		std::string name;
		std::vector<Mesh*> meshes;
		glm::mat4 modelMatrix = glm::mat4(1.0);
		Pose pose;
		glm::vec3 scale{ 1.0, 1.0, 1.0 };

		Sprite(Sprite* sprite);

	public:
		Sprite(Model* model);
		Sprite(std::vector<Mesh*> meshes);
		void setName(std::string name) { this->name = name; }
		std::string getName() { return name; }

		virtual void renderWorldSpace(Scene* scene);
		virtual void renderViewSpace(Scene* scene);

		Sprite* clone()
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

		virtual glm::vec3 getPosition();
		glm::quat getOrientation();
		glm::vec3 getScale();

		Mesh* getMesh(int i = 0);
		BaseMaterial* getMaterial(int i = 0);

		void forEachMesh(std::function<void(Mesh*)> fn)
		{
			std::for_each(meshes.begin(), meshes.end(), fn);
		}
	};

} // Audace

#endif // AU_SPRITE_H
