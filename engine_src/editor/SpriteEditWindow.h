#ifndef AU_SPRITEEDITWINDOW_H
#define AU_SPRITEEDITWINDOW_H

#include "renderer/Sprite.h"
#include "SpriteData.h"
#include "scene/Scene.h"

namespace Audace
{
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

		void editorCellFloat(std::string label, float* val, float interval);
		void editorCellAngle(std::string label, float* val, float interval);
	};
}

#endif