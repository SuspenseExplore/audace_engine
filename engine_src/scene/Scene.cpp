#include "Scene.h"
#include "application/BaseAppController.h"
#include "renderer/Sprite.h"

namespace Audace {
	Scene::Scene(BaseAppController* controller) {
		appController = controller;
	}
	void Scene::addSprite(Sprite* s) {
		for (Sprite* sprite : sprites)
		{
			if (s == sprite)
			{
				return;
			}
		}
		sprites.push_back(s);
	}

	void Scene::removeSprite(Sprite* s)
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
}