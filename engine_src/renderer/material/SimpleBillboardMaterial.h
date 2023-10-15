#ifndef AU_SIMPLEBILLBOARDMATERIAL_H
#define AU_SIMPLEBILLBOARDMATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "BaseMaterial.h"

namespace Audace {
	class SimpleBillboardMaterial : public BaseMaterial {
		std::string name;
		ShaderProgram *shaderProgram;
		Texture2d *texture;
		glm::vec4 color;

		public:
		void setName(std::string name) {this->name = name;}
		std::string getName() {return name;}
		ShaderProgram* getShader() {return shaderProgram;}
		void setShader(ShaderProgram *shaderProgram) {this->shaderProgram = shaderProgram;}
		Texture2d *getTexture() {return texture;}
		void setTexture(Texture2d *texture) {this->texture = texture;}
		glm::vec4 getColor() {return color;}
		void setColor(glm::vec4 color) {this->color = color;}

		void apply() override {
			texture->bind(0);
			shaderProgram->bind();
			shaderProgram->setUniformInt("tex1", 0);
			shaderProgram->setUniformVec4("color", glm::value_ptr(color));
		}
	};
}

#endif