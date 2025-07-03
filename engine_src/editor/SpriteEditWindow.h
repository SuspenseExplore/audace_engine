#ifndef AU_SPRITEEDITWINDOW_H
#define AU_SPRITEEDITWINDOW_H

#include "glm/glm.hpp"

namespace Audace
{
	class Sprite;
	class SpriteData;
	class Scene;

	class SpriteEditWindow
	{
		SpriteData* spriteData;
		Sprite* positionMark;

		glm::vec3 angles;

	public:
		SpriteEditWindow();

		void setSprite(SpriteData* sd);
		void renderWorldSpace(Scene* scene);
		void renderViewSpace(Scene* scene);

		void txWidgets(Scene* scene);
		void translateButton(Scene* scene, glm::vec3 diff, std::string label);

		void editorCellFloat(std::string label, float* val, float interval);
		void editorCellAngle(std::string label, float* val, float interval);
	};
}

#endif