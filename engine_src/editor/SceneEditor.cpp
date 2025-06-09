#include "SceneEditor.h"
#include <fstream>
#include "content/AssetStore.h"
#include "renderer/ShaderProgram.h"
#include "FileLoader.h"
#include "imgui.h"

namespace Audace
{
	void SceneEditor::load(std::string path, std::string filename)
	{
		if (scene == nullptr)
		{
			AU_ENGINE_LOG_ERROR("Editor can't load; scene is null");
			return;
		}

		json j = fileLoader->textFileToJson(path + filename);
		sceneData.filepath = path;
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
								if (el1.contains("scale"))
								{
									sd.scale = JsonSerializer::getVec3(el1["scale"]);
								}
								else
								{
									sd.scale = glm::vec3(1.0, 1.0, 1.0);
								}

								sd.filename = name;
								sd.pose = p;

								ShaderProgram* shader = AssetStore::getShader("obj_mtl");
								sd.sprite = AssetStore::cloneSprite(name);
								sd.sprite->setName(name);
								sd.sprite->setPosition(p.position);
								sd.sprite->setOrientation(p.orientation);
								sd.sprite->setScale(sd.scale);
								sd.sprite->forEachMesh([=](Mesh* mesh)
									{
										mesh->getMaterial()->setShader(shader);
									});
								scene->addSprite(sd.sprite);
								sceneData.spriteData.emplace_back(sd);
							});
					});

			});
	}

	void SceneEditor::save(std::string path, std::string filename)
	{
		json j = {};
		j["filename"] = filename;
		j["clearColor"] = { sceneData.clearColor.r, sceneData.clearColor.g, sceneData.clearColor.b, sceneData.clearColor.a };

		json jSprites = {};
		for (SpriteData sd : sceneData.spriteData)
		{
			json jPose = {
				{sd.pose.position.x, sd.pose.position.y, sd.pose.position.z},
				{sd.pose.orientation.w, sd.pose.orientation.x, sd.pose.orientation.y, sd.pose.orientation.z} };
			json jobj = {};
			jobj["pose"] = jPose;
			jobj["scale"] = { sd.scale.x, sd.scale.y, sd.scale.z };
			jSprites[sd.filename].emplace_back(jobj);
		}
		j["sprites"] = jSprites;

		std::ofstream fout(path + filename);
		fout << j.dump(4);
		fout.close();
	}

	void SceneEditor::syncToScene()
	{
		scene->setClearColor(sceneData.clearColor);
	}

	void SceneEditor::loadSprite(std::string name)
	{
		ShaderProgram* shader = AssetStore::getShader("obj_mtl");
		SpriteData sd;
		sd.filename = name;
		sd.sprite = AssetStore::cloneSprite(name);
		sd.sprite->setName(name);
		sd.sprite->forEachMesh([=](Mesh* mesh)
			{
				mesh->getMaterial()->setShader(shader);
			});
		scene->addSprite(sd.sprite);
		sceneData.spriteData.emplace_back(sd);
	}

	void SceneEditor::renderWorldSpace(Scene* scene)
	{
		if (selectedSprite != -1 && selectedSprite < sceneData.spriteData.size())
		{
			editWin.renderWorldSpace(scene);
		}
	}

	void SceneEditor::sceneEditWindow()
	{
		if (selectedSprite != -1 && selectedSprite < sceneData.spriteData.size())
		{
			editWin.renderViewSpace(scene);
		}

		ImGui::Begin("Scene Editor");
		ImGui::SetWindowPos(ImVec2(600, 800), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_Once);

		if (ImGui::BeginTabBar("SceneTabs"))
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				ImGui::Text("Name: %s", sceneData.filename.c_str());
				ImGui::DragFloat4("Clear color", glm::value_ptr(sceneData.clearColor), 0.01, 0.0, 1.0);
				if (ImGui::Button("Save"))
				{
					// TODO: be able to save to a better place than the build folder
					int i = sceneData.filename.find_last_of("/");
					save(fileLoader->fileWriteBasePath() + "scenes/", sceneData.filename.substr(i + 1));
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Models"))
			{
				fileListingPane(modelIndex);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Sprites"))
			{
				if (ImGui::BeginListBox("SpriteList"))
				{
					for (int i = 0; i < sceneData.spriteData.size(); i++)
					{
						ImGui::PushID(i);
						Audace::Sprite* s = sceneData.spriteData[i].sprite;
						bool selected = (i == selectedSprite);
						if (ImGui::Selectable(s->getName().c_str(), selected))
						{
							selected = true;
							selectedSprite = i;
							editWin.setSprite(&sceneData.spriteData[i]);
						}
						if (selected)
						{
							ImGui::SetItemDefaultFocus();
						}
						ImGui::PopID();
					}

					ImGui::EndListBox();
				}
				if (ImGui::Button("Remove") && selectedSprite > -1 && selectedSprite < sceneData.spriteData.size())
				{
					scene->removeSprite(sceneData.spriteData[selectedSprite].sprite);
					sceneData.spriteData.erase(sceneData.spriteData.begin() + selectedSprite);
					selectedSprite = -1;
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void SceneEditor::fileListingPane(json fileIndex, std::string currPath)
	{
		json folders = fileIndex["folders"];
		json files = fileIndex["files"];
		for (auto& i : folders.items())
		{
			if (ImGui::TreeNode(i.key().c_str()))
			{
				fileListingPane(i.value(), currPath + i.key() + "/");

				ImGui::TreePop();
			}
		}

		int c = 0;
		for (std::string i : files)
		{
			ImGui::PushID(c++);
			if (ImGui::SmallButton("Add"))
			{
				loadSprite(currPath + i);
				selectedSprite = sceneData.spriteData.size() - 1;
				editWin.setSprite(&sceneData.spriteData[selectedSprite]);
			}
			ImGui::SameLine();
			ImGui::Text("%s", i.c_str());
			ImGui::PopID();
		}
	}
}