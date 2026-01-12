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
	SceneEditor::SceneEditor(IFileAccess *fileLoader) : fileLoader(fileLoader), gui(fileLoader, "ui/scene_editor.json")
	{
		modelIndex = fileLoader->textFileToJson("models/_index.json");
		editWin = new NodeEditWindow(fileLoader);

		cubeSprite = AssetStore::getCubeSprite();
		reinterpret_cast<SimpleBillboardMaterial *>(cubeSprite->getMesh()->getMaterial())->setTexture(AssetStore::darkGridTexture());

		gui.addBinding("Clear Color", &clearColor);
		gui.addBinding("Ambient Color", &ambientColor);
		gui.addBinding("Visualize", &visualize);
		gui.addBinding("Preset", &postProcIndex);
		gui.addBinding("kernel 0, 1, 2", &postProcKernel[0]);
		gui.addBinding("kernel 3, 4, 5", &postProcKernel[3]);
		gui.addBinding("kernel 6, 7, 8", &postProcKernel[6]);
		gui.addBinding("Offset Scale", &offsetScale);

		reloadSceneFn = [=]()
		{
			scene->reloadScene();
		};
		selectNodeFn = [=](SceneGraphNode *node)
		{
			selectedNode = node;
			editWin->setNode(node);
		};
		newChildFn = [=](SceneGraphNode *node)
		{
			new SceneGraphNode(node);
		};
		addCubeFn = [=](SceneGraphNode *node)
		{
			if (node->getSprite() == nullptr)
			{
				node->setSprite(cubeSprite);
				cubeSprite->addInst(node);
			}
		};
		applyPostProcFn = [=]()
		{
			ShaderProgram *shader = AssetStore::getShader("AU_post_proc");
			shader->bind();
			shader->setUniformFloatArray("kernel[0]", postProcKernel, 9);
			shader->setUniformFloat("offsetScale", offsetScale);
		};
		gui.addBinding("Reload Scene", &reloadSceneFn);
		gui.addBinding("Select", &selectNodeFn);
		gui.addBinding("New Child", &newChildFn);
		gui.addBinding("Add Cube", &addCubeFn);
		gui.addBinding("Apply", &applyPostProcFn);
	}

	void SceneEditor::renderWorldSpace(Scene *scene)
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
			editWin->renderWorldSpace(scene);
		}
	}

	void SceneEditor::sceneEditWindow()
	{
		if (selectedNode != nullptr)
		{
			editWin->renderViewSpace(scene);
		}
		int ppi = postProcIndex;
		gui.render();
		if (ppi != postProcIndex)
		{
			switch (postProcIndex)
			{
			case 0: // Normal
				postProcKernel[0] = 0;
				postProcKernel[1] = 0;
				postProcKernel[2] = 0;
				postProcKernel[3] = 0;
				postProcKernel[4] = 1;
				postProcKernel[5] = 0;
				postProcKernel[6] = 0;
				postProcKernel[7] = 0;
				postProcKernel[8] = 0;
				offsetScale = 1;
				break;

			case 1: // Sharpen
				postProcKernel[0] = -1;
				postProcKernel[1] = -1;
				postProcKernel[2] = -1;
				postProcKernel[3] = -1;
				postProcKernel[4] = 9;
				postProcKernel[5] = -1;
				postProcKernel[6] = -1;
				postProcKernel[7] = -1;
				postProcKernel[8] = -1;
				offsetScale = 1;
				break;

			case 2: // Blur
				postProcKernel[0] = 1.0 / 16.0;
				postProcKernel[1] = 2.0 / 16.0;
				postProcKernel[2] = 1.0 / 16.0;
				postProcKernel[3] = 2.0 / 16.0;
				postProcKernel[4] = 4.0 / 16.0;
				postProcKernel[5] = 2.0 / 16.0;
				postProcKernel[6] = 1.0 / 16.0;
				postProcKernel[7] = 2.0 / 16.0;
				postProcKernel[8] = 1.0 / 16.0;
				offsetScale = 1.5;
				break;

			case 3: // Edge detect
				postProcKernel[0] = 1;
				postProcKernel[1] = 1;
				postProcKernel[2] = 1;
				postProcKernel[3] = 1;
				postProcKernel[4] = -8;
				postProcKernel[5] = 1;
				postProcKernel[6] = 1;
				postProcKernel[7] = 1;
				postProcKernel[8] = 1;
				offsetScale = 1;
				break;
			}
		}
		scene->setClearColor(clearColor);
		scene->setAmbientLight(ambientColor);
	}

	void SceneEditor::attachToScene(Scene *scene)
	{
		this->scene = scene;
		// scene->addSprite(cubeSprite);
	}

	void SceneEditor::setSceneGraph(SceneGraph *graph)
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