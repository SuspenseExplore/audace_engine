#ifndef AU_SCENEEDITOR_H
#define AU_SCENEEDITOR_H

#include <string>
#include "editor/SceneData.h"
#include "editor/SpriteEditWindow.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Audace
{
	class IFileAccess;
	class Scene;
	class SpriteEditWindow;
	class SceneGraph;

	class SceneEditor
	{
		std::string imagePath = "images/";
		IFileAccess* fileLoader;
		Scene* scene;
		SceneGraph* sceneGraph;

		int selectedSprite = -1;
		SpriteEditWindow editWin;

		json modelIndex; // lists content of the assets/models/ folder
		std::vector<std::string> modelIndexPath; // the currently selected path in modelIndex

	public:
		SceneEditor(IFileAccess* fileLoader);
		void load(std::string path, std::string filename);
		void save(std::string path, std::string filename);

		void attachToScene(Scene* scene);
		void setSceneGraph(SceneGraph* graph);
		void syncToScene();

		void loadSprite(std::string name);

		void renderWorldSpace(Scene* scene);
		void sceneEditWindow();
		void fileListingPane(json fileIndex, std::string currPath = "");
	};
}

#endif