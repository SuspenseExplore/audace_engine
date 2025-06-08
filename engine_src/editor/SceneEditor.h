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

	public:
		SceneEditor(IFileLoader* fileLoader) : fileLoader(fileLoader) {}
		void load(std::string path, std::string filename);
		void save(std::string path, std::string filename);

		void attachToScene(Scene* scene) { this->scene = scene; }
		void syncToScene();

		void renderWorldSpace(Scene* scene);
		void sceneEditWindow();
	};
}

#endif