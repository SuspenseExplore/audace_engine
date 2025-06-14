#ifndef AU_SCENE_H
#define AU_SCENE_H

#include <vector>
#include "application/BaseAppController.h"
#include "content/IFileAccess.h"
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
		virtual void loadAssets(Audace::IFileAccess* fileLoader) = 0;
		virtual void render() = 0;
		virtual void renderUi() {}
		virtual void disposeAssets() = 0;

		virtual BaseCamera* getCamera() = 0;
		virtual void setCamera(BaseCamera* camera) = 0;
		virtual void teleport(glm::vec3 pos) {}

		virtual void addSprite(Sprite* s) {
			sprites.push_back(s);
		}

		virtual void removeSprite(Sprite* s)
		{
			for (auto iter = sprites.begin(); iter != sprites.end(); iter++)
			{
				if (*iter == s)
				{
					sprites.erase(iter);
					return;
				}
			}
		}

		virtual void setClearColor(glm::vec4 color) {}
		virtual void setAmbientLight(glm::vec4 color) {}
		virtual void setDirLight(glm::vec3 dir, glm::vec4 color) {}
		virtual void setPointLight(int i, glm::vec3 pos, glm::vec4 color) {}
	};
}

#endif