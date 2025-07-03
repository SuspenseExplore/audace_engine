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
		setScale({ 0.02f, 0.02f, 0.02f });
		name = "PointLight";
	}

	void PointLight::renderWorldSpace(Scene* scene)
	{
		reinterpret_cast<Audace::SimpleBillboardMaterial*>(meshes[0]->getMaterial())->setColor({ color, 1 });
		Sprite::renderWorldSpace(scene);
	}
}