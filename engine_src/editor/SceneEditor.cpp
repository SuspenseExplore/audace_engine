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
	SceneEditor::SceneEditor(IFileAccess* fileLoader) : fileLoader(fileLoader), gui(fileLoader->textFileToJson("ui/scene_editor.json"))
	{
		modelIndex = fileLoader->textFileToJson("models/_index.json");

		gui.addBinding("Clear Color", &clearColor);
		gui.addBinding("Ambient Color", &ambientColor);
		gui.addBinding("Visualize", &visualize);
		selectNodeFn = [=](Audace::SceneGraphNode* node)
			{
				selectedNode = node;
				editWin.setNode(node);
			};
		gui.addBinding("Select Node", &selectNodeFn);
	}

	void SceneEditor::renderWorldSpace(Scene* scene)
	{
		if (visualize)
		{
			glDisable(GL_DEPTH_TEST);
			sceneGraph->getRootNode()->debugRender(scene, true);
			glEnable(GL_DEPTH_TEST);
		}
		else if (selectedNode != nullptr)
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
		gui.render();
		scene->setClearColor(clearColor);
		scene->setAmbientLight(ambientColor);
	}

	void SceneEditor::attachToScene(Scene* scene)
	{
		this->scene = scene;
	}

	void SceneEditor::setSceneGraph(SceneGraph* graph)
	{
		sceneGraph = graph;
		gui.addBinding("scene_graph", graph);
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