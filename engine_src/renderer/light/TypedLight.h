#ifndef AU_TYPEDLIGHT_H
#define AU_TYPEDLIGHT_H

#include <string>

namespace Audace
{
	class PointLight;
	class DirLight;
	class SpotLight;

	enum LightType
	{
		POINT_LIGHT, DIRECTIONAL_LIGHT, SPOTLIGHT
	};

	// TypedLight* owned by the Scene
	struct TypedLight
	{
		std::string name;
		LightType type;
		union {
			PointLight* ptLight;
			DirLight* dirLight;
			SpotLight* spotLight;
		};

		TypedLight(const std::string& name, PointLight* ptLight);
		TypedLight(const std::string& name, DirLight* dirLight);
		TypedLight(const std::string& name, SpotLight* spotLight);
	};
}

#endif