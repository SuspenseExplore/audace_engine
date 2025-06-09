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
		Pose pose = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0}};
		glm::vec3 scale = {1.0, 1.0, 1.0};
		Sprite* sprite;

		void syncToSprite();
	};
}

#endif