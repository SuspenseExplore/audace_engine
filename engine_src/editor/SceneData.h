#ifndef AU_SCENEDATA_H
#define AU_SCENEDATA_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "SpriteData.h"
#include "renderer/Sprite.h"

namespace Audace
{
	struct SceneData
	{
		std::string filename;
		std::string filepath;
		glm::vec4 clearColor;
		std::vector<SpriteData> spriteData;
	};
}

#endif