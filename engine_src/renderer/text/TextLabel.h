#ifndef AU_TEXTLABEL_H
#define AU_TEXTLABEL_H

#include <string>
#include "BitmapFont.h"
#include "math/Pose.h"
#include "scene/Scene.h"
#include "glm/glm.hpp"

namespace Audace {
	class TextLabel {
	protected:
		std::string text;
		Pose pose;
		glm::vec3 scale = glm::vec3(1, 1, 1);
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		glm::vec2 textSize;

		BitmapFont *font;

	public:
		TextLabel(BitmapFont *font, std::string text) : font(font)
		{
			setText(text);
		}

		void setText(std::string &text);

		virtual void render(Scene *scene);

		virtual void setPosition(glm::vec3 position)
		{
			pose.position = position;
		}

		virtual void setOrientation(glm::quat orientation)
		{
			pose.orientation = orientation;
		}

		virtual void setScale(glm::vec3 scale)
		{
			this->scale = scale;
		}

		glm::vec3 &getPosition() { return pose.position; }
		glm::quat &getOrientation() { return pose.orientation; }
		glm::vec3 &getScale() { return scale; }
		glm::vec4 &getColor() { return color; }

	};
}

#endif