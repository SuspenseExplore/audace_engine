#include "SceneEditor.h"
#include <fstream>
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"

namespace Audace
{
	void SceneEditor::load(std::string filename)
	{
		if (scene == nullptr)
		{
			AU_ENGINE_LOG_ERROR("Editor can't load; scene is null");
			return;
		}

		json j = fileLoader->textFileToJson(filename);
		sceneData.filename = j["filename"];
		sceneData.clearColor = JsonSerializer::getVec4(j, "clearColor");
		sceneData.spriteData.clear();

		JsonSerializer::forEach(j, "sprites", [=](std::string name, json& val)
			{
				// name is filename, val is array of objects

				JsonSerializer::forEach(val, [=](json& el1)
					{
						// el1 is a sprite object that contains a pose

						JsonSerializer::ifContains(el1, "pose", [=](json& el2)
							{
								Pose p;
								SpriteData sd;

								p.position = JsonSerializer::getVec3(el2[0]);
								p.orientation = JsonSerializer::getQuat(el2[1]);

								sd.filename = name;
								sd.pose = p;
								sceneData.spriteData.emplace_back(sd);

								ShaderProgram* shader = AssetStore::getShader("obj_mtl");
								Sprite* sp = AssetStore::cloneSprite(name);
								sp->setPosition(p.position);
								sp->setOrientation(p.orientation);
								sp->forEachMesh([=](Mesh* mesh)
									{
										mesh->getMaterial()->setShader(shader);
									});
								scene->addSprite(sp);
							});
					});

			});
	}

	void SceneEditor::save(std::string filename)
	{
		json j = {};
		j["filename"] = filename;
		j["clearColor"] = { sceneData.clearColor.r, sceneData.clearColor.g, sceneData.clearColor.b, sceneData.clearColor.a };

		json jSprites = {};
		for (SpriteData sd : sceneData.spriteData)
		{
			json jPose = {
				{sd.pose.position.x, sd.pose.position.y, sd.pose.position.z},
				{sd.pose.orientation.x, sd.pose.orientation.y, sd.pose.orientation.z, sd.pose.orientation.w} };
			jSprites[sd.filename].emplace_back(json{ "pose", jPose });
		}
		j["sprites"] = jSprites;

		std::ofstream fout(filename);
		fout << j.dump(4);
		fout.close();
	}
}