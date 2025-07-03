#include "SceneGraphNode.h"
#include "renderer/Sprite.h"
#include "INodeAnimation.h"

namespace Audace
{
	SceneGraphNode::SceneGraphNode() : parent(nullptr)
	{
	}

	SceneGraphNode::SceneGraphNode(SceneGraphNode* parent)
		: parent(parent)
	{
		parent->addChild(this);
	}

	void SceneGraphNode::setSprite(Sprite* s)
	{
		sprite = s;
	}

	Sprite* SceneGraphNode::getSprite()
	{
		return sprite;
	}

	void SceneGraphNode::addChild(SceneGraphNode* c)
	{
		children.emplace_back(c);
	}

	std::vector<SceneGraphNode*>& SceneGraphNode::getChildren()
	{
		return children;
	}

	void SceneGraphNode::addAnimation(INodeAnimation* a)
	{
		a->setNode(this);
		animations.emplace_back(a);
	}

	void SceneGraphNode::setTranslation(glm::vec3 v) { translation = v; }
	void SceneGraphNode::setScale(glm::vec3 v) { scale = v; }
	void SceneGraphNode::setRotation(glm::quat q) { rotation = q; }

	glm::vec3 SceneGraphNode::getPosition()
	{
		return glm::vec3(localTransform[3][0], localTransform[3][1], localTransform[3][2]);
	}

	void SceneGraphNode::update(glm::mat4 parentTransform)
	{
		for (INodeAnimation* a : animations)
		{
			a->update();
		}
		glm::mat4 T = glm::translate(glm::mat4(1.0), translation);
		glm::mat4 R = glm::mat4_cast(rotation);
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
		localTransform = parentTransform * T * R * S;

		// this is abusing some stuff to make my Sprite class work in this scene graph.
		// the Sprite's stored transformation data is left as identities,
		// and the model matrix is used to apply the scene graph transform.
		if (sprite != nullptr)
		{
			sprite->setModelMatrix(localTransform);
		}

		for (SceneGraphNode* child : children)
		{
			child->update(localTransform);
		}
	}
}