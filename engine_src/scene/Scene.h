#ifndef AU_SCENE_H
#define AU_SCENE_H

#include <vector>
#include "application/BaseAppController.h"
#include "FileLoader.h"
#include "scene/BaseCamera.h"
#include "renderer/Sprite.h"

namespace Audace
{
	class Sprite;

	class Scene
	{
	protected:
		BaseAppController* appController;
		std::vector<Sprite*> sprites;

	public:
		Scene(BaseAppController* controller);
		virtual ~Scene() {}
		virtual void loadAssets(Audace::FileLoader* fileLoader) = 0;
		virtual void render() = 0;
		virtual void renderUi() {}
		virtual void disposeAssets() = 0;

		virtual BaseCamera* getCamera() = 0;
		virtual void setCamera(BaseCamera* camera) = 0;
		virtual void teleport(glm::vec3 pos) {}

		virtual void addSprite(Sprite* s) {
			sprites.push_back(s);
		}

		virtual void setClearColor(glm::vec4 color) {}
	};
}

#endif