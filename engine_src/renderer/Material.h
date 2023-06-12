#ifndef AU_MATERIAL_H
#define AU_MATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "ShaderProgram.h"

namespace Audace
{
	class Material
	{
		ShaderProgram *shaderProgram;
		std::string name;
		glm::vec3 ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
		float shininess = 0.2f * 128;

	public:
		void setName(std::string name) {this->name = name;}
		std::string getName() {return name;}
		void setAmbientColor(glm::vec3 color) { ambientColor = color; }
		void setDiffuseColor(glm::vec3 color) { diffuseColor = color; }
		void setSpecularColor(glm::vec3 color) { specularColor = color; }
		void setShininess(float shininess) {this->shininess = shininess;}
		void setShader(ShaderProgram *shaderProgram) {this->shaderProgram = shaderProgram;}

		void apply() {
			shaderProgram->bind();
			shaderProgram->setUniformVec3("material.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
			shaderProgram->setUniformVec3("material.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
			shaderProgram->setUniformVec3("material.specular", specularColor.x, specularColor.y, specularColor.z);
			shaderProgram->setUniformFloat("material.shininess", shininess);
		}
	};
}

#endif