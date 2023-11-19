#ifndef AU_MATERIAL_H
#define AU_MATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "BaseMaterial.h"

namespace Audace
{
	class Material : public BaseMaterial
	{
		ShaderProgram *shaderProgram;
		std::string name;
		glm::vec3 ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
		Texture2d *ambientOcclusionMap = nullptr;
		glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
		Texture2d *diffuseMap = nullptr;
		Texture2d *normalMap = nullptr;
		glm::vec3 specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
		Texture2d *specularMap = nullptr;
		glm::vec3 emissionColor = {0, 0, 0};

	public:
		void setName(std::string name) { this->name = name; }
		std::string getName() override { return name; }
		void setAmbientColor(glm::vec3 color) { ambientColor = color; }
		glm::vec3 getAmbientColor() { return ambientColor; }
		void setAmbientOcclusionMap(Texture2d *texture) { ambientOcclusionMap = texture; }
		void setDiffuseColor(glm::vec3 color) { diffuseColor = color; }
		glm::vec3 getDiffuseColor() { return diffuseColor; }
		void setDiffuseMap(Texture2d *texture) { diffuseMap = texture; }
		void setNormalMap(Texture2d *texture) { normalMap = texture; }
		void setSpecularColor(glm::vec3 color) { specularColor = color; }
		glm::vec3 getSpecularColor() { return specularColor; }
		void setSpecularMap(Texture2d *texture) { specularMap = texture; }
		void setEmissionColor(glm::vec3 color) { emissionColor = color; }
		glm::vec3 getEmissionColor() { return emissionColor; }
		ShaderProgram *getShader() override { return shaderProgram; }
		void setShader(ShaderProgram *shaderProgram) override { this->shaderProgram = shaderProgram; }

		void apply() override
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
	};
}

#endif