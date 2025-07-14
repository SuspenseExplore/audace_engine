#include "SceneEditor.h"
#include <fstream>
#include "content/AssetStore.h"
#include "content/JsonSerializer.h"
#include "content/gltf/GltfLoader.h"
#include "editor/SpriteData.h"
#include "renderer/Mesh.h"
#include "renderer/ShaderProgram.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"
#include "content/IFileAccess.h"
#include "scene/Scene.h"
#include "scene/graph/SceneGraph.h"
#include "scene/graph/SceneGraphNode.h"
#include "imgui.h"
#include "glm/gtc/quaternion.hpp"

namespace Audace
{
	SceneEditor::SceneEditor(IFileAccess* fileLoader) : fileLoader(fileLoader)
	{
		modelIndex = fileLoader->textFileToJson("models/_index.json");

		// Audace::PointLight* pointLight = new Audace::PointLight();
		// pointLight->setPosition({ 0, -1, 1 });
		// pointLight->setColor({ 1, 1, 1 });
		// pointLight->setIntensity(1);
		// sceneData.ptLights.emplace_back(pointLight);

	}

	void SceneEditor::renderWorldSpace(Scene* scene)
	{
		if (selectedNode != nullptr)
		{
			glDisable(GL_DEPTH_TEST);
			selectedNode->debugRender(scene, false);
			glEnable(GL_DEPTH_TEST);
		}
		if (selectedNode != nullptr)
		{
			editWin.renderWorldSpace(scene);
		}
	}

	void SceneEditor::sceneEditWindow()
	{
		if (selectedNode != nullptr)
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
				sceneEditPane();
				// if (ImGui::Button("Save"))
				// {
				// 	int i = sceneData.filename.find_last_of("/");
				// 	save(fileLoader->fileWriteBasePath() + "scenes/", sceneData.filename.substr(i + 1));
				// }
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Graph"))
			{
				sceneGraphPane();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::End();
		// 		if (ImGui::BeginTabItem("Models"))
		// 		{
		// 			fileListingPane(modelIndex);

		// 			ImGui::EndTabItem();
		// 		}

	}

	void SceneEditor::sceneEditPane()
	{
		static glm::vec4 clearColor = { 0.5, 0.5, 0.5, 0.5 };
		ImGui::DragFloat4("Clear color", glm::value_ptr(clearColor), 0.01, 0.0, 1.0);
		scene->setClearColor(clearColor);

		glm::vec4 light = scene->getAmbientLight();
		ImGui::ColorPicker4("Ambient color", glm::value_ptr(light));
		scene->setAmbientLight(light);
	}

	void SceneEditor::sceneGraphPane()
	{
		sceneGraphTreeEntry(sceneGraph->getRootNode(), "0");
	}

	void SceneEditor::sceneGraphTreeEntry(SceneGraphNode* node, const std::string& path)
	{
		int x = 500;
		std::string n = (node->getName().length() > 0) ? node->getName() : path;
		ImGui::PushID(path.c_str());
		bool s = ImGui::TreeNode(n.c_str());
		ImGui::SameLine(x);
		if (ImGui::Button("Select"))
		{
			selectedNode = node;
			editWin.setNode(selectedNode);
		}
		ImGui::SameLine();
		if (ImGui::Button("Add child"))
		{

		}
		if (s)
		{
			std::vector<SceneGraphNode*> children = node->getChildren();
			for (int i = 0; i < children.size(); i++)
			{
				sceneGraphTreeEntry(children[i], path + "/" + std::to_string(i));
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void SceneEditor::attachToScene(Scene* scene)
	{
		this->scene = scene;
	}

	void SceneEditor::setSceneGraph(SceneGraph* graph)
	{
		sceneGraph = graph;
	}

	void SceneEditor::loadSprite(std::string name)
	{
		// SpriteData sd;
		// sd.filename = name;
		// sd.sprite = AssetStore::cloneSprite(name);
		// sd.sprite->setName(name);
		// scene->addSprite(sd.sprite);
		// sceneData.spriteData.emplace_back(sd);
	}

	void SceneEditor::fileListingPane(json fileIndex, std::string currPath)
	{
		// 	json folders = fileIndex["folders"];
		// 	json files = fileIndex["files"];
		// 	for (auto& i : folders.items())
		// 	{
		// 		if (ImGui::TreeNode(i.key().c_str()))
		// 		{
		// 			fileListingPane(i.value(), currPath + i.key() + "/");

		// 			ImGui::TreePop();
		// 		}
		// 	}

		// 	int c = 0;
		// 	for (std::string i : files)
		// 	{
		// 		ImGui::PushID(c++);
		// 		if (ImGui::SmallButton("Add"))
		// 		{
		// 			loadSprite(currPath + i);
		// 			selectedSprite = sceneData.spriteData.size() - 1;
		// 			editWin.setSprite(&sceneData.spriteData[selectedSprite]);
		// 		}
		// 		ImGui::SameLine();
		// 		ImGui::Text("%s", i.c_str());
		// 		ImGui::PopID();
		// 	}
	}
}