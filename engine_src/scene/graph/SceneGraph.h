#ifndef AU_SCENEGRAPH_H
#define AU_SCENEGRAPH_H

#include <vector>
#include "glm/glm.hpp"

using std::vector;
namespace Audace
{
	class Scene;
	class SceneGraphNode;
	class PointLight;

	class SceneGraph
	{
		Scene* scene;
		SceneGraphNode* rootNode;
		vector<PointLight*> ptLights;
		glm::mat4 IDENTITY = glm::mat4(1.0);

	public:
		SceneGraph(Scene* scene);
		void setRootNode(SceneGraphNode* node);
		SceneGraphNode* getRootNode();
		void addPointLight(PointLight* p);

		void update(Scene* scene);
		void debugRender(Scene* scene);
	};
}

#endif