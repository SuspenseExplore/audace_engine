#include "glfw3.h"
#include "au_renderer.h"
#include "AppController.h"
#include "scene/MainScene.h"
#include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"
#include "scene/BasicCameraController.h"

namespace Audace
{
	bool AppController::createWindow(int width, int height, std::string title)
	{
		window = new GameWindow(width, height, title);
		window->open();
		windowInitialized();
		return true;
	}

	void AppController::windowInitialized()
	{
		scene = new NavigationScene(this);
		mousePos = window->getMousePos();

		glEnable(GL_DEPTH_TEST);
		AU_RENDERER_LOG_TRACE("Renderer initialized");

		fileLoader = new FileLoader("../../assets/");
	}

	void AppController::runGameLoop()
	{
		AU_ENGINE_LOG_TRACE("Entering render loop");
		while (true)
		{
			window->processEvents();
			if (window->shouldClose())
			{
				shutdown();
				return;
			}
			if (nextScene != CURRENT)
			{
				startNextScene();
			}
			window->beginFrame();
			renderFrame();
			window->endFrame();
		}
		AU_ENGINE_LOG_TRACE("Exiting render loop");
	}

	void AppController::setScene(int scene)
	{
		nextScene = scene;
	}

	void AppController::startNextScene()
	{
		scene->disposeAssets();
		delete scene;

		switch (nextScene)
		{
		case NAVIGATION:
			scene = new NavigationScene(this);
			scene->loadAssets();
			break;

		case MAIN:
			scene = new MainScene(this, fileLoader);
			scene->loadAssets();
			break;

		case BUILDER:
			scene = new SceneBuilder(this, fileLoader);
			BaseCamera *camera = scene->getCamera();
			BasicCameraController *camCtl = new BasicCameraController((ForwardCamera *)camera);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(&camCtl->aimAction);
			scene->loadAssets();
			break;
		}
		nextScene = CURRENT;
	}

	void AppController::renderFrame()
	{
		scene->render();
	}

	void AppController::shutdown()
	{
		scene->disposeAssets();
		delete scene;
		window->close();
	}
}