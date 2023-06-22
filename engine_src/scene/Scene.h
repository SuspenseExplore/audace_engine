#ifndef AU_SCENE_H
#define AU_SCENE_H

#include "application/BaseAppController.h"

namespace Audace
{
	class Scene
	{
	protected:
		BaseAppController *appController;

	public:
		Scene(BaseAppController *controller);
		virtual ~Scene() {}
		virtual void loadAssets() = 0;
		virtual void render() = 0;
		virtual void disposeAssets() = 0;
	};
}

#endif