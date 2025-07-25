#include "TypedLight.h"

namespace Audace
{
	TypedLight::TypedLight(const std::string& name, PointLight* ptLight)
		: name(name), ptLight(ptLight), type(POINT_LIGHT)
	{
	}

	TypedLight::TypedLight(const std::string& name, DirLight* dirLight)
		: name(name), dirLight(dirLight), type(DIRECTIONAL_LIGHT)
	{
	}

	TypedLight::TypedLight(const std::string& name, SpotLight* spotLight)
		: name(name), spotLight(spotLight), type(SPOTLIGHT)
	{
	}
}