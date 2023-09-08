#include "au_renderer.h"
#include "NavigationScene.h"
#include "imgui.h"
#include "SceneEnum.h"

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