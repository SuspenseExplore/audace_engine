#include "au_renderer.h"
#include "SpriteEditWindow.h"
#include "content/AssetStore.h"
#include "renderer/Shapes.h"
#include "imgui.h"

namespace Audace
{
	SpriteEditWindow::SpriteEditWindow()
	{
		positionMark = new Sprite({Shapes::spherePositions(8, 8)});
		positionMark->getMesh()->setMaterial(AssetStore::simpleBillboardMaterial());
		positionMark->setScale({0.05, 0.05, 0.05});
	}

	void SpriteEditWindow::setSprite(Sprite *s)
	{
		sprite = s;
		position = sprite->getPosition();
		scale = sprite->getScale();
		angles = glm::degrees(glm::eulerAngles(sprite->getOrientation()));
	}

	void SpriteEditWindow::renderWorldSpace(Scene *scene)
	{
		positionMark->setPosition(sprite->getPosition());
		glDisable(GL_DEPTH_TEST);
		positionMark->renderWorldSpace(scene);
		glEnable(GL_DEPTH_TEST);
	}

	void SpriteEditWindow::renderViewSpace(Scene *scene)
	{
		std::vector<float> intervals = {0.1, 1, 5, 10, 15};
		std::vector<float> angleIntervals = {0.1, 1, 5, 15, 45, 90};
		static int intervalIndex = 1;
		static int angleIntervalIndex = 1;

		ImGui::Begin("Edit object");
		if (ImGui::BeginTable("Sprite Transformation", 4, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Position");
			ImGui::TableNextColumn();
			editorCellFloat("##Xpos", &position.x, intervals[intervalIndex]);
			ImGui::TableNextColumn();
			editorCellFloat("##Ypos", &position.y, intervals[intervalIndex]);
			ImGui::TableNextColumn();
			editorCellFloat("##Zpos", &position.z, intervals[intervalIndex]);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Scale");
			ImGui::TableNextColumn();
			editorCellFloat("##Xscale", &scale.x, intervals[intervalIndex]);
			ImGui::TableNextColumn();
			editorCellFloat("##Yscale", &scale.y, intervals[intervalIndex]);
			ImGui::TableNextColumn();
			editorCellFloat("##Zscale", &scale.z, intervals[intervalIndex]);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Angle");
			ImGui::TableNextColumn();
			editorCellAngle("##Xangle", &angles.x, angleIntervals[angleIntervalIndex]);
			ImGui::TableNextColumn();
			editorCellAngle("##Yangle", &angles.y, angleIntervals[angleIntervalIndex]);
			ImGui::TableNextColumn();
			editorCellAngle("##Zangle", &angles.z, angleIntervals[angleIntervalIndex]);

			ImGui::EndTable();
		}

		ImGui::PushID("intervals");
		char c[5];
		for (int i = 0; i < intervals.size(); i++)
		{
			ImGui::PushID(i);
			std::snprintf(c, 5, "%.1f", intervals[i]);
			if (ImGui::RadioButton(c, &intervalIndex, i))
			{
				intervalIndex = i;
			}
			ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::NewLine();
		ImGui::PushID("angles");
		for (int i = 0; i < angleIntervals.size(); i++)
		{
			ImGui::PushID(i);
			std::snprintf(c, 5, "%.1f", angleIntervals[i]);
			if (ImGui::RadioButton(c, &angleIntervalIndex, i))
			{
				angleIntervalIndex = i;
			}
			ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::PopID();
		ImGui::PopID();

		ImGui::End();
		sprite->setPosition(position);
		sprite->setScale(scale);
		glm::mat4 m = glm::mat4(1.0);
		m = glm::rotate(m, glm::radians(angles.x), {1, 0, 0});
		m = glm::rotate(m, glm::radians(angles.y), {0, 1, 0});
		m = glm::rotate(m, glm::radians(angles.z), {0, 0, 1});
		sprite->setOrientation(glm::quat(m));
	}

	void SpriteEditWindow::editorCellFloat(std::string label, float *val, float interval)
	{
		char c[5];
		std::snprintf(c, 5, "%.1f", interval);
		ImGui::DragFloat(label.c_str(), val, 0.01);
		if (ImGui::Button((std::string("-") + c + label).c_str()))
		{
			*val -= interval;
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("|<-") + label).c_str()))
		{
			*val = std::floor(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("->|") + label).c_str()))
		{
			*val = std::ceil(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("+") + c + label).c_str()))
		{
			*val += interval;
		}
	}

	void SpriteEditWindow::editorCellAngle(std::string label, float *val, float interval)
	{
		char c[5];
		std::snprintf(c, 5, "%.1f", interval);
		ImGui::DragFloat(label.c_str(), val, 0.1);
		if (ImGui::Button((std::string("-") + c + label).c_str()))
		{
			*val -= interval;
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("|<-") + label).c_str()))
		{
			*val = std::floor(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("->|") + label).c_str()))
		{
			*val = std::ceil(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("+") + c + label).c_str()))
		{
			*val += interval;
		}
	}
}
