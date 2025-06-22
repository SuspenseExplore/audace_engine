#include "SceneGraph.h"

namespace Audace
{
	void SceneGraph::update(Scene* scene)
	{
		for (SceneGraphNode* n : rootNodes)
		{
			n->update(IDENTITY);
		}
	}
}