#ifndef AU_JSONSERIALIZER_H
#define AU_JSONSERIALIZER_H

#include "glm/glm.hpp"
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
	};
}

#endif