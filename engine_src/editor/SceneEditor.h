#ifndef AU_SCENEEDITOR_H
#define AU_SCENEEDITOR_H

#include <string>
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

	class SceneEditor
	{
		std::string imagePath = "images/";
		IFileAccess* fileLoader;
		Scene* scene;
		SceneGraph* sceneGraph;

		SceneGraphNode* selectedNode = nullptr;
		NodeEditWindow editWin;
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
		void sceneEditPane();
		void sceneGraphPane();
		void sceneGraphTreeEntry(SceneGraphNode* node, const std::string& path);
		void fileListingPane(json fileIndex, std::string currPath = "");
	};
}

#endif