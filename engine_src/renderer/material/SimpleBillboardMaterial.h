#ifndef AU_SIMPLEBILLBOARDMATERIAL_H
#define AU_SIMPLEBILLBOARDMATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BaseMaterial.h"

namespace Audace {
	class ShaderProgram;
	class Texture2d;

	class SimpleBillboardMaterial : public BaseMaterial {
		std::string name;
		ShaderProgram *shaderProgram;
		Texture2d *texture;
		glm::vec4 color;

		public:
		void setName(std::string name);
		std::string getName() override;
		ShaderProgram* getShader() override;
		void setShader(ShaderProgram *shaderProgram) override;
		Texture2d *getTexture();
		void setTexture(Texture2d *texture);
		glm::vec4 getColor();
		void setColor(glm::vec4 color);

		void apply() override;
	};
}

#endif