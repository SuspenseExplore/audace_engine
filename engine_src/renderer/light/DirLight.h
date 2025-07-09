#ifndef AU_DIRLIGHT_H
#define AU_DIRLIGHT_H

#include "renderer/Sprite.h"
#include "glm/glm.hpp"

namespace Audace
{
	class DirLight : public Sprite
	{
	protected:
		glm::vec4 color{ 1, 1, 1, 1 };

	public:
		DirLight();

		void renderWorldSpace(Scene* scene) override;
		void renderViewSpace(Scene* scene) override {}
		void setColor(glm::vec3 color) { this->color = glm::vec4(color, this->color.a); }
		void setColor(glm::vec4 color) { this->color = color; }
		glm::vec4 getColor() { return color; }
		void setIntensity(float i) { color.a = i; }
		float getIntensity() { return color.a; }
	};
}

#endif