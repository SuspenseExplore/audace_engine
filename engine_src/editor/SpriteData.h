#ifndef AU_SPRITEDATA_H
#define AU_SPRITEDATA_H

#include <string>
#include <vector>
#include "math/Pose.h"
#include "content/JsonSerializer.h"
#include "renderer/Sprite.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Audace
{
	struct SpriteData
	{
		std::string filename;
		Pose pose;
		glm::vec3 scale;
		Sprite* sprite;

		void syncToSprite();
	};
}

#endif