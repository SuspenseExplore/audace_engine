#include "PbrMetalRoughMat.h"
#include "content/AssetStore.h"
#include "renderer/texture/Texture2d.h"
#include "renderer/ShaderProgram.h"

namespace Audace
{
	PbrMetalRoughMat::PbrMetalRoughMat()
	{
		baseColorMap = AssetStore::getWhiteTexture();
		normalMap = AssetStore::getBlueTexture();
		metallicMap = AssetStore::getWhiteTexture();
		roughnessMap = AssetStore::getWhiteTexture();
		occlusionMap = AssetStore::getWhiteTexture();
		emissiveMap = AssetStore::getBlackTexture();
	}
	void PbrMetalRoughMat::setBaseColorFactor(glm::vec4 f)
	{
		baseColorFactor = f;
	}

	void PbrMetalRoughMat::setMetallicFactor(float f)
	{
		metallicFactor = f;
	}

	void PbrMetalRoughMat::setRoughnessFactor(float f)
	{
		roughnessFactor = f;
	}

	void PbrMetalRoughMat::setEmissiveFactor(glm::vec3 f)
	{
		emissiveFactor = f;
	}

	void PbrMetalRoughMat::setBaseColorMap(Texture2d *t)
	{
		baseColorMap = t;
	}

	void PbrMetalRoughMat::setNormalMap(Texture2d *t)
	{
		normalMap = t;
	}

	void PbrMetalRoughMat::setMetallicMap(Texture2d *t)
	{
		metallicMap = t;
	}

	void PbrMetalRoughMat::setRoughnessMap(Texture2d *t)
	{
		roughnessMap = t;
	}

	void PbrMetalRoughMat::setOcclusionMap(Texture2d *t)
	{
		occlusionMap = t;
	}

	void PbrMetalRoughMat::setEmissiveMap(Texture2d *t)
	{
		emissiveMap = t;
	}

	std::string PbrMetalRoughMat::getName()
	{
		return name;
	}

	ShaderProgram *PbrMetalRoughMat::getShader()
	{
		return shaderProgram;
	}

	void PbrMetalRoughMat::setShader(ShaderProgram *shader)
	{
		shaderProgram = shader;
	}

	void PbrMetalRoughMat::apply()
	{
		shaderProgram->bind();
		shaderProgram->setUniformVec4("material.baseColorFactor", baseColorFactor);
		shaderProgram->setUniformFloat("material.metallicFactor", metallicFactor);
		shaderProgram->setUniformFloat("material.roughnessFactor", roughnessFactor);
		shaderProgram->setUniformVec3("material.emissiveFactor", emissiveFactor);

		if (baseColorMap != nullptr)
		{
			shaderProgram->setUniformInt("material.baseColorMap", 0);
			baseColorMap->bind(0);
		}
		if (roughnessMap != nullptr)
		{
			shaderProgram->setUniformInt("material.roughnessMap", 1);
			roughnessMap->bind(1);
		}
		if (metallicMap != nullptr)
		{
			shaderProgram->setUniformInt("material.metallicMap", 2);
			metallicMap->bind(2);
		}
		if (normalMap != nullptr)
		{
			shaderProgram->setUniformInt("material.normalMap", 3);
			normalMap->bind(3);
		}
		if (occlusionMap != nullptr)
		{
			shaderProgram->setUniformInt("material.occlusionMap", 4);
			occlusionMap->bind(4);
		}
		if (emissiveMap != nullptr)
		{
			shaderProgram->setUniformInt("material.emissiveMap", 5);
			emissiveMap->bind(5);
		}
	}
}