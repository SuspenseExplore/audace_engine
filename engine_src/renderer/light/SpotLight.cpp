#include "SpotLight.h"
#include "content/AssetStore.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/material/SimpleBillboardMaterial.h"

namespace Audace
{
	SpotLight::SpotLight() : Sprite({ Shapes::spherePositions(16, 16) })
	{
		Audace::SimpleBillboardMaterial* material = AssetStore::simpleBillboardMaterial();
		material->setTexture(AssetStore::getWhiteTexture());
		meshes[0]->setMaterial(material);
		setScale({ 0.2f, 0.2f, 0.2f });
		name = "SpotLight";
	}

	void SpotLight::renderWorldSpace(Scene* scene)
	{
		reinterpret_cast<Audace::SimpleBillboardMaterial*>(meshes[0]->getMaterial())->setColor(color);
		Sprite::renderWorldSpace(scene);
	}
}