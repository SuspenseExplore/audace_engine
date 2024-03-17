#ifndef AU_NAVIGATIONSCENE_H
#define AU_NAVIGATIONSCENE_H

#include "application/BaseAppController.h"
#include "scene/Scene.h"
#include "scene/BaseCamera.h"

class NavigationScene : public Audace::Scene
{

public:
	NavigationScene(Audace::BaseAppController* controller) : Audace::Scene(controller) {}
	void loadAssets(Audace::FileLoader *fileLoader) override {}
	void render() override;
	void disposeAssets() override {}

	Audace::BaseCamera* getCamera() override {return nullptr;}
	void setCamera(Audace::BaseCamera *camera) override {}
};

#endif