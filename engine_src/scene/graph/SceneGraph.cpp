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

	void SceneGraph::setRootNode(SceneGraphNode* node)
	{
		rootNode = node;
	}

	SceneGraphNode* SceneGraph::getRootNode()
	{
		return rootNode;
	}

	void SceneGraph::addLight(int id, Sprite* light, string type)
	{
		lights[id] = light;
		lightTypes[id] = type;
	}

	void SceneGraph::update(Scene* scene)
	{
		rootNode->update(scene, IDENTITY);
		for (int i = 0; i < lights.size(); i++)
		{
			scene->setLight(i, lights[i], lightTypes[i]);
		}
	}

	void SceneGraph::debugRender(Scene* scene)
	{
		rootNode->debugRender(scene, true);
	}
}