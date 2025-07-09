#ifndef AU_POINTLIGHT_H
#define AU_POINTLIGHT_H

#include "glm/glm.hpp"
#include "renderer/Sprite.h"

namespace Audace
{
	class Scene;

	class PointLight : public Sprite
	{
	protected:
		glm::vec3 color{ 1, 1, 1 };
		float intensity = 1;

	public:
		PointLight();

		void renderWorldSpace(Scene* scene) override;
		void renderViewSpace(Scene* scene) override {}
		void setColor(glm::vec3 color) { this->color = color; }
		glm::vec3 getColor() { return color; }
		void setIntensity(float i) { intensity = i; }
		float getIntensity() { return intensity; }
	};
}

#endif