#ifndef AU_SCENEDATA_H
#define AU_SCENEDATA_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Audace
{
	struct SpriteData;
	class PointLight;

	struct SceneData
	{
		std::string filename;
		std::string filepath;
		glm::vec4 clearColor = {0.5, 0.5, 0.5, 0.5};
		std::vector<SpriteData> spriteData;

		glm::vec4 ambLightColor = {0.1, 0.1, 0.1, 0.3};
		glm::vec3 dirLightDir;
		glm::vec4 dirLightColor;
		std::vector<PointLight*> ptLights;
	};
}

#endif