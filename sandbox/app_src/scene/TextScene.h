#ifndef AU_TEXTSCENE_H
#define AU_TEXTSCENE_H

#include "scene/Scene.h"
#include "scene/ForwardCamera.h"
#include "renderer/text/TextLabel.h"
#include "math/Pose.h"
#include "glm/glm.hpp"

class TextScene : public Audace::Scene {
	Audace::FileLoader *fileLoader;
	Audace::ForwardCamera *camera;
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 0);
	Audace::BitmapFont *font;
	Audace::TextLabel *label;

public:
	TextScene(Audace::BaseAppController *controller, Audace::FileLoader *fileLoader)
		: Scene(controller), fileLoader(fileLoader), 
		camera(Audace::ForwardCamera::standard2d(glm::vec3(0, 0, -2), appController->getWidth(), appController->getHeight()))
	{
	}

	void loadAssets(Audace::FileLoader *fileLoader) override;
	void render() override;
	void disposeAssets() override;
	Audace::BaseCamera *getCamera() override { return camera; }
	void setCamera(Audace::BaseCamera *camera) override {}
};

#endif