#ifndef AU_PBRMETALROUGHMAT_H
#define AU_PBRMETALROUGHMAT_H

#include "BaseMaterial.h"
#include "glm/glm.hpp"
#include "renderer/Texture2d.h"
#include "renderer/ShaderProgram.h"

namespace Audace
{
	class PbrMetalRoughMat : public BaseMaterial
	{
		ShaderProgram* shaderProgram;
		std::string name = "default_pbr_metal_roughness_mat";

		glm::vec4 baseColorFactor = {1, 1, 1, 1};
		float metallicFactor = 0;
		float roughnessFactor = 0.5;

		Texture2d* baseColorMap = nullptr;
		Texture2d* normalMap = nullptr;
		Texture2d* metallicMap = nullptr;
		Texture2d* roughnessMap = nullptr;

	public:
		PbrMetalRoughMat();
		void setBaseColorFactor(glm::vec4 f);
		void setMetallicFactor(float f);
		void setRoughnessFactor(float f);
		void setBaseColorMap(Texture2d* t);
		void setNormalMap(Texture2d* t);
		void setMetallicMap(Texture2d* t);
		void setRoughnessMap(Texture2d* t);

		std::string getName() override;
		ShaderProgram* getShader() override;
		void setShader(ShaderProgram* shader) override;
		void apply() override;

	};
}

#endif