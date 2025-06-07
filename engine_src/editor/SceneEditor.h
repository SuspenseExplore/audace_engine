#ifndef AU_SCENEEDITOR_H
#define AU_SCENEEDITOR_H

#include "content/IFileLoader.h"
#include "scene/Scene.h"
#include "SceneData.h"

namespace Audace
{
	class SceneEditor
	{
		IFileLoader* fileLoader;
		Scene *scene;

	public:
		std::string filename;
		SceneData sceneData;

		SceneEditor(IFileLoader* fileLoader) : fileLoader(fileLoader) {}
		void load(std::string filename);
		void save(std::string filename);

		void attachToScene(Scene *scene) {this->scene = scene;}
	};
}

#endif