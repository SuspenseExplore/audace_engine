#ifndef AU_TEXTSCENE_H
#define AU_TEXTSCENE_H

#include "scene/Scene.h"
#include "glm/glm.hpp"

namespace Audace
{
	class BaseAppController;
	class ForwardCamera;
	class BitmapFont;
	class TextLabel;
}

class TextScene : public Audace::Scene {
	Audace::IFileAccess* fileLoader;
	Audace::ForwardCamera* camera;
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 0);
	Audace::BitmapFont* font;
	Audace::TextLabel* label;

public:
	TextScene(Audace::BaseAppController* controller, Audace::IFileAccess* fileLoader);

	void loadAssets(Audace::IFileAccess* fileLoader) override;
	void render() override;
	void disposeAssets() override;
	Audace::BaseCamera* getCamera() override;
	void setCamera(Audace::BaseCamera* camera) override;
};

#endif