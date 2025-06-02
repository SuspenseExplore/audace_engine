#include "SceneDescriptor.h"
#include "content/JsonSerializer.h"
#include "content/AssetStore.h"

namespace Audace
{
	void SceneDescriptor::loadSceneAssets(IFileLoader *fileLoader)
	{
		jsonDescriptor = fileLoader->textFileToJson(descriptorFilename);
		json &sceneDesc = jsonDescriptor["scene"];

		if (sceneDesc.contains("clearColor"))
		{
			clearColor = JsonSerializer::getVec4(sceneDesc["clearColor"]);
		}

		if (sceneDesc.contains("modelMats"))
		{
			json &jsonMats = sceneDesc["modelMats"];
			for (auto &el : jsonMats.items())
			{
				std::string name = el.key();
				auto &matArray = el.value();
				for (auto &m : matArray)
				{
					glm::mat4 m = JsonSerializer::getMatrix(matArray);
					modelMats[name] = m;
				}
			}
		}

		if (sceneDesc.contains("shaders"))
		{
			json &jsonShaders = sceneDesc["shaders"];
			for (auto &el : jsonShaders)
			{
				shaders[el] = AssetStore::getShader(el);
			}
		}

		if (sceneDesc.contains("sprites"))
		{
			json &jsonModels = sceneDesc["sprites"];
			for (auto &el : jsonModels.items())
			{
				std::string name = el.key();
				json &spriteArr = el.value();
				for (auto &el2 : spriteArr)
				{
					Sprite *s = AssetStore::cloneSprite(name);
					s->setName(name);

					if (el2.contains("modelMat"))
					{
						std::string mName = el2["modelMat"];
						s->setModelMatrix(modelMats[mName]);
					}
					if (el2.contains("pose"))
					{
						json &poseJson = el2["pose"];
						s->setPosition(JsonSerializer::getVec3(poseJson[0]));
						s->setOrientation(JsonSerializer::getQuat(poseJson[1]));
					}
					if (el2.contains("shader"))
					{
						s->forEachMesh([=](Mesh *m)
									   {
									ShaderProgram *shader = shaders[el2["shader"]];
									 m->getMaterial()->setShader(shader); });
					}
					sprites.emplace_back(s);
				}
			}
		}
	}

	void SceneDescriptor::apply(Scene *scene)
	{
	}
}
