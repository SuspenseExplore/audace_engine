#include <algorithm>
#include "TextLabel.h"
#include "renderer/Shapes.h"
#include "renderer/Mesh.h"
#include "glm/gtc/type_ptr.hpp"

namespace Audace
{
	void TextLabel::setText(std::string &text)
	{
		this->text = text;
		textSize.x = 0;
		textSize.y = 0;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = font->getChar(*c);
			textSize.x += (ch.advance >> 6);
			textSize.y = std::max(ch.bearing.y, textSize.y);
		}
		textSize.y = -textSize.y;
	}

	void TextLabel::render(Scene *scene)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderProgram *shader = font->getShader();
		shader->bind();
		shader->setUniformInt("tex1", 0);
		shader->setUniformVec4("color", glm::value_ptr(color));
		glm::vec2 pos = textSize / -2.0f;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = font->getChar(*c);

			glm::mat4 worldMat = scene->getCamera()->getViewProjMatrix();
			worldMat = glm::translate(worldMat, glm::vec3(pos.x + ch.bearing.x, pos.y - ch.bearing.y, 0.0f) * scale);
			worldMat = glm::translate(worldMat, pose.position);
			worldMat *= glm::mat4_cast(pose.orientation);
			worldMat = glm::scale(worldMat, scale * glm::vec3(ch.size.x, ch.size.y, 1.0f));

			shader->setUniformMat4("txMat", glm::value_ptr(worldMat));
			ch.texture->bind(0);

			Mesh *mesh = Shapes::squarePositions();
			mesh->render();

			pos.x += (ch.advance >> 6);
		}
	}
}