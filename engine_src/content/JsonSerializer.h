#ifndef AU_JSONSERIALIZER_H
#define AU_JSONSERIALIZER_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "FileLoader.h"
#include "renderer/material/Material.h"

namespace Audace
{
	class JsonSerializer
	{
	public:
		static Material *loadMaterial(json jcontent);

		static glm::vec3 getVec3(json jcontent);
		static glm::vec4 getVec4(json jcontent);
		static glm::quat getQuat(json jcontent);
		static glm::mat4 getMatrix(json &jcontent);
		static glm::mat4 getTranslation(json jcontent);
		static glm::mat4 getRotationEulerDegrees(json jcontent);
	};
}

#endif