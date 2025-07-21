#ifndef AU_SCENEEDITOR_H
#define AU_SCENEEDITOR_H

#include <map>
#include <string>
#include "content/JsonGui.h"
#include "editor/SceneData.h"
#include "editor/NodeEditWindow.h"
#include "glm/glm.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class IFileAccess;
	class Scene;
	class SceneGraph;
	class SceneGraphNode;
	class Sprite;

	class SceneEditor
	{
		std::string imagePath = "images/";
		IFileAccess* fileLoader;
		Scene* scene;
		SceneGraph* sceneGraph;
		JsonGui gui;
		std::function<void(SceneGraphNode*)> selectNodeFn;
		std::function<void(SceneGraphNode*)> newChildFn;
		std::function<void(SceneGraphNode*)> addCubeFn;

		Sprite* cubeSprite;
		std::map<int, SceneGraphNode*> insts;

		SceneGraphNode* selectedNode = nullptr;
		NodeEditWindow* editWin;
		glm::vec4 clearColor = { 0.5, 0.5, 0.5, 0.5 };
		glm::vec4 ambientColor = { 1.0, 1.0, 1.0, 0.03 };
		bool visualize = false;

		json modelIndex; // lists content of the assets/models/ folder
		std::vector<std::string> modelIndexPath; // the currently selected path in modelIndex

	public:
		SceneEditor(IFileAccess* fileLoader);

		void attachToScene(Scene* scene);
		void setSceneGraph(SceneGraph* graph);

		void loadSprite(std::string name);

		void renderWorldSpace(Scene* scene);
		void sceneEditWindow();
		void fileListingPane(json fileIndex, std::string currPath = "");
	};
}

#endif