#ifndef AU_SPOTLIGHT_H
#define AU_SPOTLIGHT_H

#include "glm/glm.hpp"
#include "renderer/Sprite.h"

namespace Audace
{
	class Scene;

	class SpotLight : public Sprite
	{
	protected:
		glm::vec4 color{ 1, 1, 1, 1 };
		float innerAngle = 0;
		float outerAngle = glm::pi<float>() / 4.0;

	public:
		SpotLight();

		void renderWorldSpace(Scene* scene) override;
		void renderViewSpace(Scene* scene) override {}
		void setColor(glm::vec3 color)
		{
			this->color.r = color.r;
			this->color.g = color.g;
			this->color.b = color.b;
		}
		glm::vec3 getColor() { return glm::vec3(color); }
		void setIntensity(float i) { color.a = i; }
		float getIntensity() { return color.a; }
		void setInnerAngle(float f) { innerAngle = f; }
		float getInnerAngle() { return innerAngle; }
		void setOuterAngle(float f) { outerAngle = f; }
		float getOuterAngle() { return outerAngle; }
	};
}

#endif