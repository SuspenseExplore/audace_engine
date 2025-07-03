#ifndef AU_SCENEGRAPH_H
#define AU_SCENEGRAPH_H

#include <vector>
#include "glm/glm.hpp"

using std::vector;
namespace Audace
{
	class Scene;
	class SceneGraphNode;

	class SceneGraph
	{
		Scene* scene;
		vector<SceneGraphNode*> rootNodes;
		glm::mat4 IDENTITY = glm::mat4(1.0);

	public:
		SceneGraph(Scene* scene);
		void addRootNode(SceneGraphNode* node);
		SceneGraphNode* getRootNode();

		void update(Scene* scene);
	};
}

#endif