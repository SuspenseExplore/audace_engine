#include "SimpleBillboardMaterial.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"

namespace Audace {
	void SimpleBillboardMaterial::setName(std::string name)
	{
		this->name = name;
	}

	std::string SimpleBillboardMaterial::getName()
	{
		return name;
	}

	ShaderProgram* SimpleBillboardMaterial::getShader()
	{
		return shaderProgram;
	}

	void SimpleBillboardMaterial::setShader(ShaderProgram* shaderProgram)
	{
		this->shaderProgram = shaderProgram;
	}

	Texture2d* SimpleBillboardMaterial::getTexture()
	{
		return texture;
	}

	void SimpleBillboardMaterial::setTexture(Texture2d* texture)
	{
		this->texture = texture;
	}

	glm::vec4 SimpleBillboardMaterial::getColor()
	{
		return color;
	}

	void SimpleBillboardMaterial::setColor(glm::vec4 color)
	{
		this->color = color;
	}

	void SimpleBillboardMaterial::apply()
	{
		texture->bind(0);
		shaderProgram->bind();
		shaderProgram->setUniformInt("tex1", 0);
		shaderProgram->setUniformVec4("color", glm::value_ptr(color));
	}

}