#ifndef AU_NODEEDITWINDOW_H
#define AU_NODEEDITWINDOW_H

#include "content/JsonGui.h"
#include "glm/glm.hpp"

namespace Audace
{
	class IFileAccess;
	class Sprite;
	class SpriteData;
	class Scene;
	class SceneGraphNode;

	class NodeEditWindow
	{
		SceneGraphNode* node;
		int nodeType;
		Sprite* positionMark;

		IFileAccess* fileLoader;
		JsonGui* jsonGui;
		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 angles;
		glm::vec4 lightColor;
		float spotlightInnerAngle;
		float spotlightOuterAngle;

	public:
		NodeEditWindow(IFileAccess* fileLoader);

		void setNode(SceneGraphNode* node);
		void renderWorldSpace(Scene* scene);
		void renderViewSpace(Scene* scene);

		void txWidgets(Scene* scene);
		void moveWidgetButton(Scene* scene, glm::vec3 diff, std::string label);
	};
}

#endif