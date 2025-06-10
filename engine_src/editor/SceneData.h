#ifndef AU_SCENEDATA_H
#define AU_SCENEDATA_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "SpriteData.h"
#include "renderer/light/PointLight.h"

namespace Audace
{
	struct SceneData
	{
		std::string filename;
		std::string filepath;
		glm::vec4 clearColor;
		std::vector<SpriteData> spriteData;

		glm::vec4 ambLightColor;
		glm::vec3 dirLightDir;
		glm::vec4 dirLightColor;
		std::vector<PointLight*> ptLights;
	};
}

#endif