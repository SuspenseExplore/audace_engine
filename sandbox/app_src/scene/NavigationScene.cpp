#include "au_renderer.h"
#include "application/BaseAppController.h"
#include "NavigationScene.h"
#include "imgui.h"
#include "SceneEnum.h"

NavigationScene::NavigationScene(Audace::BaseAppController* controller)
	: Audace::Scene(controller)
{

}

void NavigationScene::loadAssets(Audace::IFileAccess* fileLoader)
{

}

void NavigationScene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Begin("Scenes");
	if (ImGui::Button("Main Scene")) {
		appController->setScene(SandboxScene::MAIN);
	}
	if (ImGui::Button("Scene Builder")) {
		appController->setScene(SandboxScene::BUILDER);
	}
	if (ImGui::Button("Drag & Drop")) {
		appController->setScene(SandboxScene::DRAG_DROP);
	}
	if (ImGui::Button("Text")) {
		appController->setScene(SandboxScene::TEXT);
	}
	ImGui::End();
}

Audace::BaseCamera* NavigationScene::getCamera()
{
	return nullptr;
}

void NavigationScene::setCamera(Audace::BaseCamera* camera)
{

}

void NavigationScene::disposeAssets()
{

}
