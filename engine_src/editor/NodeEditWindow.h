#ifndef AU_NODEEDITWINDOW_H
#define AU_NODEEDITWINDOW_H

#include "glm/glm.hpp"

namespace Audace
{
	class Sprite;
	class SpriteData;
	class Scene;
	class SceneGraphNode;

	class NodeEditWindow
	{
		SceneGraphNode* node;
		Sprite* positionMark;

		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 angles;

	public:
		NodeEditWindow();

		void setNode(SceneGraphNode* node);
		void renderWorldSpace(Scene* scene);
		void renderViewSpace(Scene* scene);

		void txWidgets(Scene* scene);
		void translateButton(Scene* scene, glm::vec3 diff, std::string label);

		void editorCellFloat(std::string label, float* val, float interval);
		void editorCellAngle(std::string label, float* val, float interval);
	};
}

#endif