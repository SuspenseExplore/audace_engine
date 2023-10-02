#ifndef AU_SCENE_H
#define AU_SCENE_H

#include "application/BaseAppController.h"
#include "FileLoader.h"
#include "scene/BaseCamera.h"

namespace Audace
{
	class Scene
	{
	protected:
		BaseAppController *appController;

	public:
		Scene(BaseAppController *controller);
		virtual ~Scene() {}
		virtual void loadAssets(Audace::FileLoader *fileLoader) = 0;
		virtual void render() = 0;
		virtual void disposeAssets() = 0;

		virtual BaseCamera* getCamera() = 0;
		virtual void setCamera(BaseCamera *camera) = 0;
	};
}

#endif