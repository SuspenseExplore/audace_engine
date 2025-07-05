#include "SceneGraph.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraphNode.h"
#include "renderer/light/PointLight.h"

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

	void SceneGraph::addPointLight(PointLight* p)
	{
		ptLights.emplace_back(p);
	}

	void SceneGraph::update(Scene* scene)
	{
		for (SceneGraphNode* n : rootNodes)
		{
			n->update(IDENTITY);
		}
		for (int i = 0; i < ptLights.size(); i++)
		{
			PointLight* p = ptLights[i];
			scene->setPointLight(i, p->getPosition(), glm::vec4(p->getColor(), p->getIntensity()));
		}
	}
}