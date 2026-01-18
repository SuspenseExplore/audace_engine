#include <string>
#include "GameWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glfw3.h"
#include "input/ButtonInputEvent.h"
#include "input/Vec2InputEvent.h"

namespace Audace
{
	GameWindow::GameWindow(int width, int height, std::string title)
	{
		this->width = width;
		this->height = height;
		this->title = title;
	}

	bool GameWindow::open()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
		AU_RENDERER_LOG_TRACE("GLAD loaded");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.FontGlobalScale = 2;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 100");
		// ImGui::GetStyle().ScaleAllSizes(2.0);

		KeyboardManager::setStaticRef(&keyboardManager);
		glfwSetKeyCallback(window, KeyboardManager::eventCallback);

		MouseManager::setStaticRef(&mouseManager);
		glfwSetMouseButtonCallback(window, MouseManager::buttonEventCallback);
		glfwSetCursorPosCallback(window, MouseManager::moveEventCallback);
		glfwSetScrollCallback(window, MouseManager::wheelEventCallback);
		MouseManager::setMouseWheelEventHandler(
			[=](Vec2InputEvent e)
			{
				ImGuiIO &io = ImGui::GetIO();
				io.AddMouseWheelEvent(e.state.x, e.state.y);
			});
		return true;
	}

	bool GameWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void GameWindow::close()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void GameWindow::processEvents()
	{
		glfwPollEvents();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		int pressed = glfwGetMouseButton(window, 0);
		ImGuiIO &io = ImGui::GetIO();
		io.AddMousePosEvent(x, y);
		io.AddMouseButtonEvent(0, pressed == GLFW_PRESS);
	}

	void GameWindow::beginFrame()
	{
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GameWindow::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		int err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
		}
		glfwSwapBuffers(window);
	}

	void GameWindow::addKeyEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
	{
		keyboardManager.addButtonEventHandler(button, handler);
	}

	void GameWindow::addMouseButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
	{
		mouseManager.addButtonEventHandler(button, handler);
	}

	void GameWindow::setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> handler)
	{
		mouseManager.setMouseMoveEventHandler(handler);
	}

	bool GameWindow::isMouseButtonDown(int button) { return glfwGetMouseButton(window, button) == GLFW_PRESS; };

	glm::vec2 GameWindow::getMousePos()
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return glm::vec2(x, y);
	}

	int GameWindow::getWidth() { return width; }
	int GameWindow::getHeight() { return height; }
}