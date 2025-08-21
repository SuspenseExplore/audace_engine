#include "SceneGraph.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraphNode.h"
#include "au_renderer.h"

namespace Audace
{
	SceneGraph::SceneGraph()
	{
		rootNode = new SceneGraphNode();
		rootNode->setName("root");
	}

	void SceneGraph::setRootNode(SceneGraphNode *node)
	{
		rootNode = node;
	}

	SceneGraphNode *SceneGraph::getRootNode()
	{
		return rootNode;
	}

	void SceneGraph::update(Scene *scene)
	{
		rootNode->update(scene, IDENTITY);
	}

	void SceneGraph::debugRender(Scene *scene)
	{
		rootNode->debugRender(scene, true);
	}

	void SceneGraph::dispose()
	{
		rootNode->dispose();
		delete rootNode;
	}
}