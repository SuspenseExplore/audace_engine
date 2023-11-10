#include "JsonSerializer.h"
#include "content/AssetStore.h"

namespace Audace
{
	Material *JsonSerializer::loadMaterial(json jcontent)
	{
		Material *mat = new Material();
		mat->setName(jcontent["name"]);
		mat->setShader(AssetStore::getShader(jcontent["shaderName"]));
		mat->setAmbientOcclusionMap(AssetStore::getTexture(jcontent["ambientMap"]));
		mat->setAmbientColor(getVec3(jcontent["ambientColor"]));
		mat->setDiffuseMap(AssetStore::getTexture(jcontent["diffuseMap"]));
		mat->setNormalMap(AssetStore::getTexture(jcontent["normalMap"]));
		mat->setDiffuseColor(getVec3(jcontent["diffuseColor"]));
		mat->setSpecularMap(AssetStore::getTexture(jcontent["roughnessMap"]));
		mat->setSpecularColor(getVec3(jcontent["specularColor"]));
		mat->setEmissionColor(getVec3(jcontent["emissionColor"]));

		return mat;
	}

	glm::vec3 JsonSerializer::getVec3(json jcontent)
	{
		return {jcontent[0], jcontent[1], jcontent[2]};
	}
}
