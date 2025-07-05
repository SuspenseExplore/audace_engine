#ifndef AU_SCENEGRAPHNODE_H
#define AU_SCENEGRAPHNODE_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Audace
{
	class INodeAnimation;
	class Sprite;
	class Scene;

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

		static Sprite* debugAxes;

	public:
		SceneGraphNode();
		SceneGraphNode(SceneGraphNode* parent);
		void setSprite(Sprite* s);
		Sprite* getSprite();
		void addChild(SceneGraphNode* c);
		std::vector<SceneGraphNode*>& getChildren();
		void addAnimation(INodeAnimation* a);

		void setTranslation(glm::vec3 v);
		void setScale(glm::vec3 v);
		void setRotation(glm::quat q);
		glm::vec3 getPosition();
		void update(glm::mat4 parentTransform);

		void debugRender(Scene* scene);
	};
}

#endif