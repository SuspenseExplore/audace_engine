#ifndef AU_SCENE_H
#define AU_SCENE_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Audace
{
	class Sprite;
	class BaseAppController;
	class IFileAccess;
	class BaseCamera;
	class PointLight;
	class DirLight;

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

		virtual void addSprite(Sprite* s);
		virtual void removeSprite(Sprite* s);

		virtual void setClearColor(glm::vec4 color) {}
		virtual void setAmbientLight(glm::vec4 color) {}
		virtual glm::vec4 getAmbientLight() { return { 1, 0, 1, 1 }; }
		virtual void setLight(int id, Sprite* light, std::string type) {}
		virtual Sprite* getLight(int id) { return nullptr; }
	};
}

#endif