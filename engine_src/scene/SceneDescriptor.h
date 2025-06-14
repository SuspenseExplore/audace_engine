#ifndef AU_SCENEDESCRIPTOR_H
#define AU_SCENEDESCRIPTOR_H

#include <string>
#include <vector>
#include <map>
#include "au_renderer.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "Scene.h"
#include "content/IFileAccess.h"
#include "glm/glm.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Audace
{
	class SceneDescriptor
	{
		std::string descriptorFilename;
		json jsonDescriptor;
		int nextSpriteId = 0;

	public:
		glm::vec4 clearColor{ 0.5, 0, 0.5, 1.0 };
		std::map<std::string, glm::mat4> modelMats;
		std::map<std::string, ShaderProgram*> shaders;
		std::vector<Sprite*> sprites;

		SceneDescriptor(std::string filename) : descriptorFilename(filename) {}

		void loadSceneAssets(IFileAccess* fileLoader);
		void apply(Scene* scene);
	};
}

#endif