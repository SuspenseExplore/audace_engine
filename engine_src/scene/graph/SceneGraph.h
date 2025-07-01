#ifndef AU_SCENEGRAPH_H
#define AU_SCENEGRAPH_H

#include <vector>
#include "scene/Scene.h"
#include "scene/graph/SceneGraphNode.h"

using std::vector;
namespace Audace
{
	class SceneGraph
	{
		Scene* scene;
		vector<SceneGraphNode*> rootNodes;
		glm::mat4 IDENTITY = glm::mat4(1.0);

	public:
		SceneGraph(Scene* scene) : scene(scene) {}
		void addRootNode(SceneGraphNode* node) { rootNodes.emplace_back(node); }
		SceneGraphNode* getRootNode() { return rootNodes[0]; }

		void update(Scene* scene);
	};
}

#endif