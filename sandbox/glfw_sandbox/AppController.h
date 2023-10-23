#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <functional>
#include <string>
#include "GameWindow.h"
#include "EngineEventListener.h"
#include "application/BaseAppController.h"
#include "FileLoader.h"
#include "scene/Scene.h"
#include "SceneEnum.h"

namespace Audace
{
	class AppController : public EngineEventListener, BaseAppController
	{
		void startNextScene();

	public:
		GameWindow *window;
		FileLoader *fileLoader;
		Scene *scene = nullptr;
		int nextScene = SandboxScene::BUILDER;
		glm::vec2 mousePos;

		AppController() {}

		bool createWindow(int width, int height, std::string title);
		void processEvents();
		void runGameLoop();
		void renderFrame();
		void shutdown();

		int getWidth() override { return window->getWidth(); }
		int getHeight() override { return window->getHeight(); }
		void setScene(int newScene) override;

		void setMousePosListener(std::function<void(Vec2InputEvent)> listener)
		{
			window->setMouseMoveEventHandler(listener);
		}
		void setMouseButtonListener(int button, std::function<void(ButtonInputEvent)> listener)
		{
			window->addMouseButtonEventHandler(button, listener);
		}

		void windowInitialized() override;
	};
}

#endif