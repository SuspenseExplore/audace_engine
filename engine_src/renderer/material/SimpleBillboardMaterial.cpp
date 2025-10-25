#include "SimpleBillboardMaterial.h"
#include "renderer/ShaderProgram.h"
#include "renderer/texture/ITexture.h"

namespace Audace
{
	void SimpleBillboardMaterial::setName(std::string name)
	{
		this->name = name;
	}

	std::string SimpleBillboardMaterial::getName()
	{
		return name;
	}

	ShaderProgram *SimpleBillboardMaterial::getShader()
	{
		return shaderProgram;
	}

	void SimpleBillboardMaterial::setShader(ShaderProgram *shaderProgram)
	{
		this->shaderProgram = shaderProgram;
	}

	ITexture *SimpleBillboardMaterial::getTexture()
	{
		return texture;
	}

	void SimpleBillboardMaterial::setTexture(ITexture *texture)
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
		if (texture != nullptr)
		{
			texture->bind(0);
		}
		shaderProgram->bind();
		shaderProgram->setUniformInt("tex1", 0);
		shaderProgram->setUniformVec4("color", glm::value_ptr(color));
	}

}