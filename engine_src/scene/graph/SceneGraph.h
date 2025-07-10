#ifndef AU_SCENEGRAPH_H
#define AU_SCENEGRAPH_H

#include <string>
#include <map>
#include "glm/glm.hpp"

using std::string;
using std::map;
namespace Audace
{
	class Scene;
	class SceneGraphNode;
	class Sprite;

	class SceneGraph
	{
		Scene* scene;
		SceneGraphNode* rootNode;
		map<int, Sprite*> lights;
		map<int, string> lightTypes;
		glm::mat4 IDENTITY = glm::mat4(1.0);

	public:
		SceneGraph(Scene* scene);
		void setRootNode(SceneGraphNode* node);
		SceneGraphNode* getRootNode();
		void addLight(int id, Sprite* light, string type);

		void update(Scene* scene);
		void debugRender(Scene* scene);
	};
}

#endif