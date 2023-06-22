#ifndef AU_NAVIGATIONSCENE_H
#define AU_NAVIGATIONSCENE_H

#include "application/BaseAppController.h"
#include "scene/Scene.h"

class NavigationScene : public Audace::Scene
{

public:
	NavigationScene(Audace::BaseAppController* controller) : Audace::Scene(controller) {}
	void loadAssets() override {}
	void render() override;
	void disposeAssets() override {}
};

#endif