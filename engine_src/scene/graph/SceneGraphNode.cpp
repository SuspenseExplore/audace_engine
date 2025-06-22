#include "SceneGraphNode.h"

namespace Audace
{
	void SceneGraphNode::update(glm::mat4 parentTransform)
	{
		for (INodeAnimation* a : animations)
		{
			a->update();
			AU_ENGINE_LOG_DEBUG("{},{},{},{}", rotation.x, rotation.y, rotation.z, rotation.w);
		}
		glm::mat4 T = glm::translate(glm::mat4(1.0), translation);
		glm::mat4 R = glm::mat4_cast(rotation);
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
		localTransform = T * R * S * parentTransform;

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