#ifndef AU_SCENEGRAPHNODE_H
#define AU_SCENEGRAPHNODE_H

#include <string>
#include <vector>
#include "math/BoundingBox.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Audace
{
	class INodeAnimation;
	class ShaderProgram;
	class Sprite;
	class Scene;

	enum NodeType
	{
		NORMAL_NODE,
		SPRITE_NODE,
		DIRLIGHT_NODE,
		PTLIGHT_NODE,
		SPOTLIGHT_NODE
	};

	class SceneGraphNode
	{
		NodeType type = NORMAL_NODE;
		std::string name;
		SceneGraphNode *parent;
		std::vector<SceneGraphNode *> children;
		std::vector<INodeAnimation *> animations;

		Sprite *sprite = nullptr;
		glm::vec3 translation{0.0, 0.0, 0.0};
		glm::vec3 scale{1.0, 1.0, 1.0};
		glm::quat rotation{1.0, 0.0, 0.0, 0.0};
		glm::mat4 localTransform = glm::mat4(1.0);

		static Sprite *debugAxes;
		static Sprite *bboxSprite;
		static ShaderProgram *shader;
		BoundingBox bbox;

	public:
		SceneGraphNode();
		SceneGraphNode(SceneGraphNode *parent);
		SceneGraphNode *clone();
		void dispose();

		void setName(std::string name);
		const std::string &getName();
		void setSprite(Sprite *s, NodeType type = SPRITE_NODE);
		Sprite *getSprite();
		void addChild(SceneGraphNode *c);
		std::vector<SceneGraphNode *> &getChildren();
		void addAnimation(INodeAnimation *a);

		void setTranslation(glm::vec3 v);
		void setScale(glm::vec3 v);
		void setRotation(glm::quat q);
		glm::vec3 getTranslation() { return translation; }
		glm::vec3 getScale() { return scale; }
		glm::quat getRotation() { return rotation; }
		glm::mat4 getLocalTransform() { return localTransform; }
		glm::vec3 getPosition();
		void setBoundingBox(BoundingBox bb)
		{
			bbox.min = bb.min;
			bbox.max = bb.max;
		}
		BoundingBox getBoundingBox() { return bbox; }
		void update(Scene *scene, glm::mat4 parentTransform);

		NodeType getNodeType() { return type; }

		void debugRender(Scene *scene, bool recursive);
	};
}

#endif