#ifndef AU_MATERIAL_H
#define AU_MATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/ShaderProgram.h"
#include "BaseMaterial.h"

namespace Audace
{
	class Material : public BaseMaterial
	{
		ShaderProgram *shaderProgram;
		std::string name;
		glm::vec3 ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
		int ambientOcclusionMap;
		glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
		int diffuseMap;
		int normalMap;
		glm::vec3 specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 emissionColor = {0, 0, 0};
		float shininess = 0.2f * 128;

	public:
		void setName(std::string name) {this->name = name;}
		std::string getName() override {return name;}
		void setAmbientColor(glm::vec3 color) { ambientColor = color; }
		void setAmbientOcclusionMap(int texUnit) { ambientOcclusionMap = texUnit; }
		void setDiffuseColor(glm::vec3 color) { diffuseColor = color; }
		void setDiffuseMap(int texUnit) { diffuseMap = texUnit; }
		void setNormalMap(int texUnit) { normalMap = texUnit; }
		void setSpecularColor(glm::vec3 color) { specularColor = color; }
		void setEmissionColor(glm::vec3 color) { emissionColor = color; }
		void setShininess(float shininess) {this->shininess = shininess;}
		ShaderProgram* getShader() override {return shaderProgram;}
		void setShader(ShaderProgram *shaderProgram) override {this->shaderProgram = shaderProgram;}

		void apply() override {
			shaderProgram->bind();
			shaderProgram->setUniformVec3("material.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
			shaderProgram->setUniformInt("material.ambientMap", ambientOcclusionMap);
			shaderProgram->setUniformVec3("material.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
			shaderProgram->setUniformInt("material.diffuseMap", diffuseMap);
			shaderProgram->setUniformInt("material.normalMap", normalMap);
			shaderProgram->setUniformVec3("material.specular", specularColor.x, specularColor.y, specularColor.z);
			shaderProgram->setUniformVec3("material.emission", glm::value_ptr(emissionColor));
			shaderProgram->setUniformFloat("material.shininess", shininess);
		}
	};
}

#endif