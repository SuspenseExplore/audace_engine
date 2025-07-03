#ifndef AU_NAVIGATIONSCENE_H
#define AU_NAVIGATIONSCENE_H

#include "scene/Scene.h"

namespace Audace
{
	class BaseAppController;
}

class NavigationScene : public Audace::Scene
{

public:
	NavigationScene(Audace::BaseAppController* controller);
	void loadAssets(Audace::IFileAccess* fileLoader);
	void render() override;
	void disposeAssets() override;

	Audace::BaseCamera* getCamera() override;
	void setCamera(Audace::BaseCamera* camera) override;
};

#endif