#include "Material.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"

namespace Audace {

	void Material::apply()
	{
		shaderProgram->bind();
		shaderProgram->setUniformVec3("material.ambient", ambientColor);
		shaderProgram->setUniformVec3("material.diffuse", diffuseColor);
		shaderProgram->setUniformVec3("material.specular", specularColor);
		shaderProgram->setUniformVec3("material.emission", glm::value_ptr(emissionColor));

		if (ambientOcclusionMap != nullptr)
		{
			ambientOcclusionMap->bind(0);
			shaderProgram->setUniformInt("material.ambientMap", 0);
		}
		if (diffuseMap != nullptr)
		{
			shaderProgram->setUniformInt("material.diffuseMap", 1);
			diffuseMap->bind(1);
		}
		if (normalMap != nullptr)
		{
			shaderProgram->setUniformInt("material.normalMap", 2);
			normalMap->bind(2);
		}
		if (specularMap != nullptr)
		{
			shaderProgram->setUniformInt("material.specularMap", 3);
			specularMap->bind(3);
		}
	}
}