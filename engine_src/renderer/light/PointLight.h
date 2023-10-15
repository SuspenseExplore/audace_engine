#ifndef AU_POINTLIGHT_H
#define AU_POINTLIGHT_H

#include "content/AssetStore.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "glm/glm.hpp"

namespace Audace
{
	class PointLight : public Sprite
	{
	protected:
		glm::vec3 color;
		float intensity;

	public:
		PointLight() : Sprite({Shapes::spherePositions(16, 16)})
		{
			Audace::SimpleBillboardMaterial *material = AssetStore::simpleBillboardMaterial();
			material->setTexture(AssetStore::getWhiteTexture());
			meshes[0]->setMaterial(material);
			setScale({0.1f, 0.1f, 0.1f});
		}
		void render(Scene *scene) override
		{
			reinterpret_cast<Audace::SimpleBillboardMaterial*>(meshes[0]->getMaterial())->setColor({color, 1});
			Sprite::render(scene);
		}

		void setColor(glm::vec3 color) { this->color = color; }
		glm::vec3 getColor() { return color; }
		void setIntensity(float i) { intensity = i; }
		float getIntensity() { return intensity; }
	};
}

#endif