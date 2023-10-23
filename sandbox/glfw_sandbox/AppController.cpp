#include "glfw3.h"
#include "au_renderer.h"
#include "AppController.h"
#include "content/AssetStore.h"
#include "scene/MainScene.h"
// #include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"
// #include "scene/DragDropScene.h"
// #include "scene/TextScene.h"
#include "scene/BasicCameraController.h"
#include "scene/ForwardCamera.h"

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
		mousePos = window->getMousePos();

		glEnable(GL_DEPTH_TEST);
		AU_RENDERER_LOG_TRACE("Renderer initialized");

		fileLoader = new FileLoader("../../assets/");
		AssetStore::init(fileLoader);
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
		if (scene != nullptr)
		{
			scene->disposeAssets();
			delete scene;
		}

		switch (nextScene)
		{
			// case NAVIGATION:
			// 	scene = new NavigationScene(this);
			// 	scene->loadAssets();
			// 	break;

		case MAIN:
{			scene = new MainScene(this);
			ForwardCamera *camera = ForwardCamera::standard3d({0, 0, 1}, 1280, 720);
			BasicCameraController *camCtl = new BasicCameraController(camera);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_1,
														  [this](BooleanInputEvent event)
														  {
															  reinterpret_cast<MainScene *>(scene)->enableAmbientOcclusion(!event.state);
														  });
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
			scene->setCamera(camera);
			scene->loadAssets(fileLoader);
}			break;

			// case DRAG_DROP:
			// 	scene = new DragDropScene(this, fileLoader);
			// 	MouseManager::setMouseMoveEventHandler([this](Vec2InputEvent event)
			// 										   { ((DragDropScene *)scene)->mouseMoved(event.state.x, event.state.y); });
			// 	MouseManager::addButtonEventHandler(0, [this](ButtonInputEvent event)
			// 										{ ((DragDropScene *)scene)->buttonChanged(event.pressed); });
			// 	scene->loadAssets();
			// 	break;

			// case TEXT:
			// 	scene = new TextScene(this, fileLoader);
			// 	scene->loadAssets();
			// 	break;

			case BUILDER:
{				scene = new SceneBuilder(this);
				BaseCamera *camera = Audace::ForwardCamera::standard3d(glm::vec3(0, -10, 2), getWidth(), getHeight());
				scene->setCamera(camera);
				BasicCameraController *camCtl = new BasicCameraController((ForwardCamera *)camera);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
				KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
				MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
				MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
				scene->loadAssets(fileLoader);
}				break;
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