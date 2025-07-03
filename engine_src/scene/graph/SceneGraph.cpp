#include "SceneGraph.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraphNode.h"

namespace Audace
{
	SceneGraph::SceneGraph(Scene* scene) : scene(scene)
	{
	}

	void SceneGraph::addRootNode(SceneGraphNode* node)
	{
		rootNodes.emplace_back(node);
	}

	SceneGraphNode* SceneGraph::getRootNode()
	{
		return rootNodes[0];
	}

	void SceneGraph::update(Scene* scene)
	{
		for (SceneGraphNode* n : rootNodes)
		{
			n->update(IDENTITY);
		}
	}
}