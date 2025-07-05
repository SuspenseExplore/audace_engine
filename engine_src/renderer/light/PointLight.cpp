#include "content/AssetStore.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/light/PointLight.h"
#include "renderer/material/SimpleBillboardMaterial.h"

namespace Audace
{
	PointLight::PointLight() : Sprite({ Shapes::spherePositions(16, 16) })
	{
		Audace::SimpleBillboardMaterial* material = AssetStore::simpleBillboardMaterial();
		material->setTexture(AssetStore::getWhiteTexture());
		meshes[0]->setMaterial(material);
		setScale({ 0.2f, 0.2f, 0.2f });
		name = "PointLight";
	}

	void PointLight::renderWorldSpace(Scene* scene)
	{
		reinterpret_cast<Audace::SimpleBillboardMaterial*>(meshes[0]->getMaterial())->setColor({ color, 1 });
		Sprite::renderWorldSpace(scene);
	}

	glm::vec3 PointLight::getPosition()
	{
		glm::vec3 p = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
		return p;
	}
}