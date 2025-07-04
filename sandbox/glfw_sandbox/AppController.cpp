#include <fstream>
#include "au_renderer.h"
#include "AppController.h"
#include "GameWindow.h"
#include "input/ButtonInputEvent.h"
#include "input/BooleanInputEvent.h"
#include "input/Vec2InputEvent.h"
#include "KeyboardManager.h"
#include "FileAccessGlfw.h"
#include "content/AssetStore.h"
#include "content/Model.h"
#include "scene/MainScene.h"
#include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"
#include "scene/DragDropScene.h"
#include "scene/TextScene.h"
#include "scene/ProcTerrainScene.h"
#include "scene/GltfViewerScene.h"
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

		fileLoader = new FileAccessGlfw();
		fileLoader->setExternalFilePath("../../../assets/");

		// build an index file for models in assets/models/
		json index = json::object();
		index["folders"] = json::object();
		index["files"] = json::array();
		std::string folder;
		int i;
		std::vector<std::string> files = fileLoader->listFilesInDir("models", true);
		for (std::string s : files)
		{
			if (s.find(".gltf") == s.npos) {
				continue;
			}
			// file entries all start with "models/" and we want to skip that part
			s = s.substr(7);

			json* j = &index;
			while ((i = s.find_first_of('/')) != s.npos)
			{
				std::string f = s.substr(0, i);
				s = s.substr(i + 1);
				j = &((*j)["folders"]);
				if (!j->contains(f))
				{
					(*j)[f] = json::object();
				}
				j = &((*j)[f]);
				if (!j->contains("folders"))
				{
					(*j)["folders"] = json::object();
				}
			}
			if (!j->contains("files"))
			{
				(*j)["files"] = json::array();
			}
			if (s != "_index.json")
			{
				(*j)["files"].push_back(s);
			}
		}
		std::ofstream fout("../../../assets/models/_index.json");
		fout << index.dump(4);
		fout.close();

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
		case NAVIGATION:
			scene = new NavigationScene(this);
			scene->loadAssets(fileLoader);
			break;

		case MAIN:
		{
			scene = new MainScene(this);
			ForwardCamera* camera = ForwardCamera::standard3d({ 0, 0, 1 }, 1280, 720);
			BasicCameraController* camCtl = new BasicCameraController(camera);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_1,
				[this](BooleanInputEvent event)
				{
					reinterpret_cast<MainScene*>(scene)->enableAmbientOcclusion(!event.state);
				});
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
			scene->setCamera(camera);
			scene->loadAssets(fileLoader);
		}
		break;

		case DRAG_DROP:
			scene = new DragDropScene(this, fileLoader);
			MouseManager::setMouseMoveEventHandler([this](Vec2InputEvent event)
				{ ((DragDropScene*)scene)->mouseMoved(event.state.x, event.state.y); });
			MouseManager::addButtonEventHandler(0, [this](ButtonInputEvent event)
				{ ((DragDropScene*)scene)->buttonChanged(event.pressed); });
			scene->loadAssets(fileLoader);
			break;

		case TEXT:
			scene = new TextScene(this, fileLoader);
			scene->loadAssets(fileLoader);
			break;

		case BUILDER:
		{
			scene = new SceneBuilder(this);
			BaseCamera* camera = Audace::ForwardCamera::standard3d(glm::vec3(0, -10, 2), getWidth(), getHeight());
			scene->setCamera(camera);
			BasicCameraController* camCtl = new BasicCameraController((ForwardCamera*)camera);
			camCtl->setVelocityFactor(0.1f);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
			scene->loadAssets(fileLoader);
		}
		break;

		case PROC_TERRAIN:
		{
			scene = new ProcTerrainScene(this);
			BaseCamera* camera = Audace::ForwardCamera::standard3d(glm::vec3(0, -10, 2), getWidth(), getHeight());
			scene->setCamera(camera);
			BasicCameraController* camCtl = new BasicCameraController((ForwardCamera*)camera);
			camCtl->setVelocityFactor(0.1f);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
			scene->loadAssets(fileLoader);
		}
		break;

		case GLTF:
		{
			scene = new GltfViewerScene(this);
			BaseCamera* camera = Audace::ForwardCamera::standard3d(glm::vec3(0, -10, 2), getWidth(), getHeight());
			scene->setCamera(camera);
			BasicCameraController* camCtl = new BasicCameraController((ForwardCamera*)camera);
			camCtl->setVelocityFactor(0.1f);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_W, camCtl->forwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_S, camCtl->backwardAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_A, camCtl->leftAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_D, camCtl->rightAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Q, camCtl->upAction);
			KeyboardManager::addButtonChangedEventHandler(GLFW_KEY_Z, camCtl->downAction);
			MouseManager::addButtonChangedEventHandler(1, camCtl->rightMouseAction);
			MouseManager::setMouseMoveEventHandler(camCtl->aimAction);
			scene->loadAssets(fileLoader);
		}
		break;
		}
		nextScene = CURRENT;
	}

	void AppController::renderFrame()
	{
		scene->render();
		((ProcTerrainScene*)scene)->renderUi();
	}

	int AppController::getWidth()
	{
		return window->getWidth();
	}

	int AppController::getHeight()
	{
		return window->getHeight();
	}

	void AppController::setMousePosListener(std::function<void(Vec2InputEvent)> listener)
	{
		window->setMouseMoveEventHandler(listener);
	}
	void AppController::setMouseButtonListener(int button, std::function<void(ButtonInputEvent)> listener)
	{
		window->addMouseButtonEventHandler(button, listener);
	}

	void AppController::shutdown()
	{
		scene->disposeAssets();
		delete scene;
		window->close();
	}
}