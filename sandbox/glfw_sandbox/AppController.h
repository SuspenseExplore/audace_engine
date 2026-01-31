#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <functional>
#include <string>
#include "EngineEventListener.h"
#include "application/BaseAppController.h"
#include "SceneEnum.h"
#include "glm/glm.hpp"

namespace Audace
{
	class GameWindow;
	class FileAccessGlfw;
	class Scene;
	class Vec2InputEvent;
	class ButtonInputEvent;
	class FrameBuffer;

	class AppController : public EngineEventListener, BaseAppController
	{
		void startNextScene();

	public:
		GameWindow *window;
		FileAccessGlfw *fileLoader;
		Scene *scene = nullptr;
		int nextScene = SandboxScene::GLTF;
		glm::vec2 mousePos;

		// binocular view specific
		bool useBinocularView = false;
		float offsetVal = 0.03;
		float angleVal = 0.000;
		bool crossView = true;
		Audace::FrameBuffer *binocFrameBuffer[2];

		AppController() {}

		bool createWindow(int width, int height, std::string title);
		void processEvents();
		void runGameLoop();
		void renderFrame();
		void shutdown();

		int getWidth() override;
		int getHeight() override;
		GLuint getDefaultFramebuffer() override { return 0; }
		void setScene(int newScene) override;

		void setMousePosListener(std::function<void(Vec2InputEvent)> listener);
		void setMouseButtonListener(int button, std::function<void(ButtonInputEvent)> listener);
		void windowInitialized() override;
	};
}

#endif