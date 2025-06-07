#ifndef AU_SPRITEDATA_H
#define AU_SPRITEDATA_H

#include <string>
#include <vector>
#include "math/Pose.h"
#include "content/JsonSerializer.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Audace
{
	struct SpriteData
	{
		std::string filename;
		Pose pose;
	};
}

#endif