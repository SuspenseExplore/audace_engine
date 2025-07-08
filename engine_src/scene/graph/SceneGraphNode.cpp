#include "SceneGraphNode.h"
#include "content/AssetStore.h"
#include "renderer/Mesh.h"
#include "renderer/Sprite.h"
#include "renderer/material/SimpleBillboardMaterial.h"
#include "scene/Scene.h"
#include "INodeAnimation.h"

namespace Audace
{
	Sprite* SceneGraphNode::debugAxes = nullptr;
	SceneGraphNode::SceneGraphNode() : parent(nullptr)
	{
		if (debugAxes == nullptr)
		{
			debugAxes = AssetStore::getColoredAxes();
		}
	}

	SceneGraphNode::SceneGraphNode(SceneGraphNode* parent)
		: parent(parent)
	{
		parent->addChild(this);
		if (debugAxes == nullptr)
		{
			debugAxes = AssetStore::getColoredAxes();
		}
	}

	void SceneGraphNode::setName(std::string name)
	{
		this->name = name;
	}

	const std::string& SceneGraphNode::getName()
	{
		return name;
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
		c->parent = this;
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
			sprite->setModelMatrix(parentTransform);
			sprite->setPosition(translation);
			sprite->setScale(scale);
			sprite->setOrientation(rotation);
		}

		for (SceneGraphNode* child : children)
		{
			child->update(localTransform);
		}
	}

	void SceneGraphNode::debugRender(Scene* scene, bool recursive)
	{
		debugAxes->setModelMatrix(localTransform);
		debugAxes->renderWorldSpace(scene);
		if (recursive)
		{
			for (SceneGraphNode* c : children)
			{
				c->debugRender(scene, recursive);
			}
		}
	}
}