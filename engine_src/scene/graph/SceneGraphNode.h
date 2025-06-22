#ifndef AU_SCENEGRAPHNODE_H
#define AU_SCENEGRAPHNODE_H

#include <vector>
#include "renderer/Sprite.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "INodeAnimation.h"

namespace Audace
{
	class SceneGraphNode
	{
		SceneGraphNode* parent;
		std::vector<SceneGraphNode*> children;
		std::vector<INodeAnimation*> animations;

		Sprite* sprite = nullptr;
		glm::vec3 translation{ 0.0, 0.0, 0.0 };
		glm::vec3 scale{ 1.0, 1.0, 1.0 };
		glm::quat rotation{ 1.0, 0.0, 0.0, 0.0 };
		glm::mat4 localTransform = glm::mat4(1.0);

	public:
		SceneGraphNode(SceneGraphNode* parent = nullptr) : parent(parent) {}
		void setSprite(Sprite* s) { sprite = s; }
		Sprite* getSprite() { return sprite; }
		void addChild(SceneGraphNode* c) { children.emplace_back(c); }
		std::vector<SceneGraphNode*>& getChildren() { return children; }
		void addAnimation(INodeAnimation* a)
		{
			a->setNode(this);
			animations.emplace_back(a);
		}

		void setTranslation(glm::vec3 v) { translation = v; }
		void setScale(glm::vec3 v) { scale = v; }
		void setRotation(glm::quat q) { rotation = q; }
		void update(glm::mat4 parentTransform);
	};
}

#endif