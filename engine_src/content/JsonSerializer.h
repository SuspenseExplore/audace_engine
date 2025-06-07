#ifndef AU_JSONSERIALIZER_H
#define AU_JSONSERIALIZER_H

#include <functional>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "renderer/material/Material.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "FileLoader.h"

namespace Audace
{
	class JsonSerializer
	{
	public:
		static Material* loadMaterial(json jcontent);

		static void forEach(json jcontent, std::function<void(json& el)> fn);
		static void forEach(json jcontent, std::string name, std::function<void(json& el)> fn);
		static void forEach(json jcontent, std::string name, std::function<void(std::string name, json& val)> fn);
		static void ifContains(json jcontent, std::string name, std::function<void(json& el)> fn);

		static glm::vec3 getVec3(json jcontent);
		static glm::vec3 getVec3(json jcontent, std::string name);
		static glm::vec4 getVec4(json jcontent);
		static glm::vec4 getVec4(json jcontent, std::string name);
		static glm::quat getQuat(json jcontent);
		static glm::quat getQuat(json jcontent, std::string name);
		static glm::mat4 getMatrix(json& jcontent);
		static glm::mat4 getTranslation(json jcontent);
		static glm::mat4 getRotationEulerDegrees(json jcontent);
	};
}

#endif