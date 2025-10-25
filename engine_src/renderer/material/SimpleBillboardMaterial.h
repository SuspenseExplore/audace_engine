#ifndef AU_SIMPLEBILLBOARDMATERIAL_H
#define AU_SIMPLEBILLBOARDMATERIAL_H

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BaseMaterial.h"

namespace Audace
{
	class ShaderProgram;
	class ITexture;

	class SimpleBillboardMaterial : public BaseMaterial
	{
		std::string name;
		ShaderProgram *shaderProgram;
		ITexture *texture = nullptr;
		glm::vec4 color = {1, 1, 1, 1};

	public:
		void setName(std::string name);
		std::string getName() override;
		ShaderProgram *getShader() override;
		void setShader(ShaderProgram *shaderProgram) override;
		ITexture *getTexture();
		void setTexture(ITexture *texture);
		glm::vec4 getColor();
		void setColor(glm::vec4 color);

		void apply() override;
	};
}

#endif