#include "JsonSerializer.h"
#include "content/AssetStore.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Audace
{
	Material* JsonSerializer::loadMaterial(json jcontent)
	{
		Material* mat = new Material();
		mat->setName(jcontent["name"]);
		mat->setShader(AssetStore::getShader(jcontent["shaderName"]));
		mat->setAmbientOcclusionMap(AssetStore::getTexture(jcontent["ambientMap"]));
		mat->setAmbientColor(getVec3(jcontent["ambientColor"]));
		mat->setDiffuseMap(AssetStore::getTexture(jcontent["diffuseMap"]));
		mat->setNormalMap(AssetStore::getTexture(jcontent["normalMap"]));
		mat->setDiffuseColor(getVec3(jcontent["diffuseColor"]));
		mat->setSpecularMap(AssetStore::getTexture(jcontent["roughnessMap"]));
		mat->setSpecularColor(getVec3(jcontent["specularColor"]));
		mat->setEmissionColor(getVec3(jcontent["emissionColor"]));

		return mat;
	}

	void JsonSerializer::forEach(json jcontent, std::function<void(json& el)> fn)
	{
		for (auto& i : jcontent)
		{
			fn(i);
		}
	}

	void JsonSerializer::forEach(json jcontent, std::string name, std::function<void(json& el)> fn)
	{
		for (auto& i : jcontent[name])
		{
			fn(i);
		}
	}

	void JsonSerializer::forEach(json jcontent, std::string name, std::function<void(std::string name, json& val)> fn)
	{
		for (auto& i : jcontent[name].items())
		{
			fn(i.key(), i.value());
		}
	}

	void JsonSerializer::ifContains(json jcontent, std::string name, std::function<void(json& el)> fn)
	{
		if (jcontent.contains(name))
		{
			fn(jcontent[name]);
		}
	}
	std::string JsonSerializer::getString(json jcontent, std::string name)
	{
		return jcontent[name].template get<std::string>();
	}

	int JsonSerializer::getInt(json jcontent, std::string name)
	{
		return jcontent[name].template get<int>();
	}

	float JsonSerializer::getFloat(json jcontent, std::string name)
	{
		return jcontent[name].template get<float>();
	}

	glm::vec2 JsonSerializer::getVec2(json jcontent)
	{
		return { jcontent[0], jcontent[1] };
	}

	glm::vec2 JsonSerializer::getVec2(json jcontent, std::string name)
	{
		json j = jcontent[name];
		return { j[0], j[1] };
	}

	glm::vec3 JsonSerializer::getVec3(json jcontent)
	{
		return { jcontent[0], jcontent[1], jcontent[2] };
	}

	glm::vec3 JsonSerializer::getVec3(json jcontent, std::string name)
	{
		json j = jcontent[name];
		return { j[0], j[1], j[2] };
	}

	glm::vec4 JsonSerializer::getVec4(json jcontent)
	{
		return { jcontent[0], jcontent[1], jcontent[2], jcontent[3] };
	}

	glm::vec4 JsonSerializer::getVec4(json jcontent, std::string name)
	{
		json j = jcontent[name];
		return { j[0], j[1], j[2], j[3] };
	}

	glm::quat JsonSerializer::getQuat(json jcontent)
	{
		return { jcontent[3], jcontent[0], jcontent[1], jcontent[2] };
	}

	glm::quat JsonSerializer::getQuat(json jcontent, std::string name)
	{
		json j = jcontent[name];
		return { j[3], j[0], j[1], j[2] };
	}

	// jcontent is an array of transformations
	glm::mat4 JsonSerializer::getMatrix(json& jcontent)
	{
		glm::mat4 result = glm::mat4(1.0);
		for (int i = 0; i < jcontent.size(); i++)
		{
			auto& el = jcontent[i];
			for (auto& tx : el.items())
			{
				std::string name = tx.key();
				if (name == "offset" || name == "translate")
				{
					glm::mat4 m = getTranslation(tx.value());
					result = m * result;
				}
				else if (name == "rotateEulerDegrees")
				{
					glm::mat4 m = getRotationEulerDegrees(tx.value());
					result = m * result;
				}
			}
		}
		return result;
	}

	glm::mat4 JsonSerializer::getTranslation(json jcontent)
	{
		return glm::translate(glm::mat4(1.0), glm::vec3(jcontent[0], jcontent[1], jcontent[2]));
	}

	glm::mat4 JsonSerializer::getRotationEulerDegrees(json jcontent)
	{
		glm::mat4 result = glm::mat4(1.0);
		float x = jcontent[0];
		float y = jcontent[1];
		float z = jcontent[2];
		result = glm::rotate(result, glm::radians(x), { 1, 0, 0 });
		result = glm::rotate(result, glm::radians(y), { 0, 1, 0 });
		result = glm::rotate(result, glm::radians(z), { 0, 0, 1 });
		return result;
	}
}
