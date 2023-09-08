#include "TextScene.h"
#include "SceneEnum.h"
#include "imgui.h"

void TextScene::loadAssets()
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

void TextScene::disposeAssets() {

}