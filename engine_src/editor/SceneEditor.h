#ifndef AU_SCENEEDITOR_H
#define AU_SCENEEDITOR_H

#include "content/IFileLoader.h"
#include "scene/Scene.h"
#include "SceneData.h"
#include "SpriteEditWindow.h"

namespace Audace
{
	class SceneEditor
	{
		IFileLoader* fileLoader;
		Scene* scene;
		SceneData sceneData;

		int selectedSprite = -1;
		SpriteEditWindow editWin;

		json modelIndex; // lists content of the assets/models/ folder
		std::vector<std::string> modelIndexPath; // the currently selected path in modelIndex

	public:
		SceneEditor(IFileLoader* fileLoader) : fileLoader(fileLoader)
		{
			modelIndex = fileLoader->textFileToJson("models/_index.json");
		}
		void load(std::string path, std::string filename);
		void save(std::string path, std::string filename);

		void attachToScene(Scene* scene) { this->scene = scene; }
		void syncToScene();

		void loadSprite(std::string name);

		void renderWorldSpace(Scene* scene);
		void sceneEditWindow();
		void fileListingPane(json fileIndex, std::string currPath = "");
	};
}

#endif