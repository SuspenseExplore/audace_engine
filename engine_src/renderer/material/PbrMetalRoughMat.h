#ifndef AU_PBRMETALROUGHMAT_H
#define AU_PBRMETALROUGHMAT_H

#include "BaseMaterial.h"
#include "glm/glm.hpp"

namespace Audace
{
	class Texture2d;
	class ShaderProgram;

	class PbrMetalRoughMat : public BaseMaterial
	{
		ShaderProgram* shaderProgram;
		std::string name = "default_pbr_metal_roughness_mat";

		glm::vec4 baseColorFactor = { 1, 1, 1, 1 };
		float metallicFactor = 0.0;
		float roughnessFactor = 1.0;
		glm::vec3 emissiveFactor = { 0, 0, 0 };

		Texture2d* baseColorMap = nullptr;
		Texture2d* normalMap = nullptr;
		Texture2d* metallicMap = nullptr;
		Texture2d* roughnessMap = nullptr;
		Texture2d* occlusionMap = nullptr;
		Texture2d* emissiveMap = nullptr;

	public:
		PbrMetalRoughMat();
		void setBaseColorFactor(glm::vec4 f);
		void setMetallicFactor(float f);
		void setRoughnessFactor(float f);
		void setEmissiveFactor(glm::vec3 f);
		void setBaseColorMap(Texture2d* t);
		void setNormalMap(Texture2d* t);
		void setMetallicMap(Texture2d* t);
		void setRoughnessMap(Texture2d* t);
		void setOcclusionMap(Texture2d* t);
		void setEmissiveMap(Texture2d* t);

		std::string getName() override;
		ShaderProgram* getShader() override;
		void setShader(ShaderProgram* shader) override;
		void apply() override;

	};
}

#endif