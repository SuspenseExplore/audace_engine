#ifndef AU_SCENEGRAPH_H
#define AU_SCENEGRAPH_H

#include <string>
#include <map>
#include "glm/glm.hpp"

using std::map;
using std::string;
namespace Audace
{
	class Scene;
	class SceneGraphNode;
	class Sprite;

	class SceneGraph
	{
		Scene *scene;
		SceneGraphNode *rootNode;
		glm::mat4 IDENTITY = glm::mat4(1.0);

	public:
		SceneGraph();
		void setRootNode(SceneGraphNode *node);
		SceneGraphNode *getRootNode();

		void update(Scene *scene);
		void debugRender(Scene *scene);
		void dispose();
	};
}

#endif