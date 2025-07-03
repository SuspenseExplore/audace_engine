#include "TextScene.h"
#include "au_renderer.h"
#include "SceneEnum.h"
#include "application/BaseAppController.h"
#include "content/IFileAccess.h"
#include "renderer/text/BitmapFont.h"
#include "renderer/text/TextLabel.h"
#include "scene/ForwardCamera.h"
#include "imgui.h"

TextScene::TextScene(Audace::BaseAppController* controller, Audace::IFileAccess* fileLoader)
	: Scene(controller), fileLoader(fileLoader),
	camera(Audace::ForwardCamera::standard2d(glm::vec3(0, 0, -2), appController->getWidth(), appController->getHeight()))
{
}

void TextScene::loadAssets(Audace::IFileAccess* fileLoader)
{
	font = new Audace::BitmapFont(fileLoader, "arial.ttf");
	label = new Audace::TextLabel(font, "Sphinx of black quartz, judge my vow.");
}

void TextScene::render() {
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	label->render(this);

	ImGui::Begin("Scenes");
	if (ImGui::Button("Navigation"))
	{
		appController->setScene(SandboxScene::NAVIGATION);
	}
	if (ImGui::Button("Main"))
	{
		appController->setScene(SandboxScene::MAIN);
	}
	if (ImGui::Button("Scene Builder"))
	{
		appController->setScene(SandboxScene::BUILDER);
	}
	if (ImGui::Button("Drag & Drop"))
	{
		appController->setScene(SandboxScene::DRAG_DROP);
	}
	ImGui::End();

	ImGui::Begin("Label");
	static char text[20];
	if (ImGui::InputText("Text", text, 20)) {
		label->setText(std::string(text));
	}
	ImGui::DragFloat3("Position", glm::value_ptr(label->getPosition()));
	ImGui::DragFloat3("Orientation", glm::value_ptr(label->getOrientation()));
	ImGui::DragFloat3("Scale", glm::value_ptr(label->getScale()));
	ImGui::ColorPicker4("Color", glm::value_ptr(label->getColor()));
	ImGui::End();
}

Audace::BaseCamera* TextScene::getCamera()
{
	return camera;
}

void TextScene::setCamera(Audace::BaseCamera* camera)
{

}

void TextScene::disposeAssets() {

}