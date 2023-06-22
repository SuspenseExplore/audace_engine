#include "glfw3.h"
#include "au_renderer.h"
#include "AppController.h"
#include "scene/MainScene.h"
#include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"

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

		// window->addKeyEventHandler(GLFW_KEY_A, [this](ButtonInputEvent event)
		// 						   { reinterpret_cast<MainScene *>(scene)->setVelX(event.pressed ? -0.1f : 0); });
		// window->addKeyEventHandler(GLFW_KEY_D, [this](ButtonInputEvent event)
		// 						   { reinterpret_cast<MainScene *>(scene)->setVelX(event.pressed ? 0.1f : 0); });
		// window->addKeyEventHandler(GLFW_KEY_W, [this](ButtonInputEvent event)
		// 						   { reinterpret_cast<MainScene *>(scene)->setVelY(event.pressed ? 0.1f : 0); });
		// window->addKeyEventHandler(GLFW_KEY_S, [this](ButtonInputEvent event)
		// 						   { reinterpret_cast<MainScene *>(scene)->setVelY(event.pressed ? -0.1f : 0); });
		// window->addMouseButtonEventHandler(GLFW_MOUSE_BUTTON_1, [this](ButtonInputEvent event)
		// 								   { reinterpret_cast<MainScene *>(scene)->setLightBright(event.pressed); });
		// // window->addMouseButtonEventHandler(GLFW_MOUSE_BUTTON_2, [this](ButtonInputEvent event)
		// // 								   { if (event.pressed && event.changed) {reinterpret_cast<MainScene *>(scene)->randomLightColor();} });
		// window->setMouseMoveEventHandler([this](Vec2InputEvent event)
		// 								 { 											
		// 									// rotate the camera if scroll wheel is held down
		// 									if (event.changed && window->isMouseButtonDown(GLFW_MOUSE_BUTTON_2)) {
		// 										reinterpret_cast<MainScene *>(scene)->rotateCamera((event.state.y - mousePos.y) * 0.005f, 0, (event.state.x - mousePos.x) * 0.005f);
		// 									} else {
		// 										// move the light based on mouse pos
		// 										reinterpret_cast<MainScene *>(scene)->setLightPos(event.state.x, event.state.y);
		// 									}
		// 									mousePos = glm::vec2(event.state.x, event.state.y); });
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