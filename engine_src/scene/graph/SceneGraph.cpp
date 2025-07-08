#include "SceneGraph.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraphNode.h"
#include "renderer/light/PointLight.h"
#include "au_renderer.h"

namespace Audace
{
	SceneGraph::SceneGraph(Scene* scene) : scene(scene)
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

	void SceneGraph::addPointLight(PointLight* p)
	{
		ptLights.emplace_back(p);
	}

	void SceneGraph::update(Scene* scene)
	{
		rootNode->update(IDENTITY);
		for (int i = 0; i < ptLights.size(); i++)
		{
			PointLight* p = ptLights[i];
			scene->setPointLight(i, p);
		}
	}

	void SceneGraph::debugRender(Scene* scene)
	{
		rootNode->debugRender(scene, true);
	}
}