#include "au_renderer.h"
#include "SpriteEditWindow.h"
#include "content/AssetStore.h"
#include "content/Model.h"
#include "editor/SpriteData.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "scene/Scene.h"
#include "scene/BaseCamera.h"
#include "imgui.h"
#include "glm/gtx/string_cast.hpp"

namespace Audace
{
	SpriteEditWindow::SpriteEditWindow()
	{
		positionMark = new Sprite({ Shapes::spherePositions(8, 8) });
		positionMark->getMesh()->setMaterial(AssetStore::simpleBillboardMaterial());
		positionMark->setScale({ 0.05, 0.05, 0.05 });
	}

	void SpriteEditWindow::setSprite(SpriteData* sd)
	{
		spriteData = sd;
		angles = glm::degrees(glm::eulerAngles(sd->sprite->getOrientation()));
	}

	void SpriteEditWindow::renderWorldSpace(Scene* scene)
	{
		positionMark->setPosition(spriteData->pose.position);
		glDisable(GL_DEPTH_TEST);
		positionMark->renderWorldSpace(scene);
		glEnable(GL_DEPTH_TEST);
	}

	void SpriteEditWindow::renderViewSpace(Scene* scene)
	{
		std::vector<float> intervals = { 0.01, 0.1, 1, 5, 10, 15 };
		std::vector<float> angleIntervals = { 0.01, 0.1, 1, 5, 15, 45, 90 };
		static int intervalIndex = 2;
		static int angleIntervalIndex = 6;

		txWidgets(scene);

		ImGui::Begin("Edit object");
		if (ImGui::BeginTabBar("Sprite Tx Tabs"))
		{
			if (ImGui::BeginTabItem("Translation"))
			{
				if (ImGui::BeginTable("Sprite Translation Controls", 3, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("X");
					ImGui::TableNextColumn();
					ImGui::Text("Y");
					ImGui::TableNextColumn();
					ImGui::Text("Z");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					editorCellFloat("##Xpos", &spriteData->pose.position.x, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Ypos", &spriteData->pose.position.y, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Zpos", &spriteData->pose.position.z, intervals[intervalIndex]);

					ImGui::EndTable();
				}
				ImGui::PushID("intervals");
				char c[5];
				for (int i = 0; i < 3; i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", intervals[i]);
					if (ImGui::RadioButton(c, &intervalIndex, i))
					{
						intervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::NewLine();
				for (int i = 3; i < intervals.size(); i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", intervals[i]);
					if (ImGui::RadioButton(c, &intervalIndex, i))
					{
						intervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::PopID();

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Scale"))
			{
				if (ImGui::BeginTable("Sprite Scale Controls", 3, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("X");
					ImGui::TableNextColumn();
					ImGui::Text("Y");
					ImGui::TableNextColumn();
					ImGui::Text("Z");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					editorCellFloat("##Xscale", &spriteData->scale.x, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Yscale", &spriteData->scale.y, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Zscale", &spriteData->scale.z, intervals[intervalIndex]);

					ImGui::EndTable();
				}
				ImGui::PushID("intervals");
				char c[5];
				for (int i = 0; i < 3; i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", intervals[i]);
					if (ImGui::RadioButton(c, &intervalIndex, i))
					{
						intervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::NewLine();
				for (int i = 3; i < intervals.size(); i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", intervals[i]);
					if (ImGui::RadioButton(c, &intervalIndex, i))
					{
						intervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::PopID();

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Rotation"))
			{
				if (ImGui::BeginTable("Sprite Rotation Controls", 3, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("X");
					ImGui::TableNextColumn();
					ImGui::Text("Y");
					ImGui::TableNextColumn();
					ImGui::Text("Z");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					editorCellFloat("##Xangle", &angles.x, angleIntervals[angleIntervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Yangle", &angles.y, angleIntervals[angleIntervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Zangle", &angles.z, angleIntervals[angleIntervalIndex]);

					ImGui::EndTable();
				}
				ImGui::PushID("angles");
				char c[5];
				for (int i = 0; i < 4; i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", angleIntervals[i]);
					if (ImGui::RadioButton(c, &angleIntervalIndex, i))
					{
						angleIntervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::NewLine();
				for (int i = 4; i < angleIntervals.size(); i++)
				{
					ImGui::PushID(i);
					std::snprintf(c, 5, "%.2f", angleIntervals[i]);
					if (ImGui::RadioButton(c, &angleIntervalIndex, i))
					{
						angleIntervalIndex = i;
					}
					ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::PopID();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
		spriteData->pose.orientation = glm::quat(glm::radians(glm::vec3(angles.x, angles.y, angles.z)));
		spriteData->syncToSprite();
	}

	void SpriteEditWindow::txWidgets(Scene* scene)
	{
		glm::ivec2 p = scene->getCamera()->projectScreenSpace(spriteData->pose.position);
		ImGui::PushID("tx_widgets");
		translateButton(scene, glm::vec3(-1, 0, 0), "-X");
		translateButton(scene, glm::vec3(1, 0, 0), "+x");
		translateButton(scene, glm::vec3(0, -1, 0), "-Y");
		translateButton(scene, glm::vec3(0, 1, 0), "+Y");
		translateButton(scene, glm::vec3(0, 0, -1), "-Z");
		translateButton(scene, glm::vec3(0, 0, 1), "+Z");
		ImGui::PopID();
	}

	void SpriteEditWindow::translateButton(Scene* scene, glm::vec3 diff, std::string label)
	{
		glm::vec3 pos = spriteData->pose.position + diff;
		glm::ivec2 p = scene->getCamera()->projectScreenSpace(pos);
		ImGui::SetNextWindowPos(ImVec2(p.x, p.y));
		ImGui::SetNextWindowSize(ImVec2(50, 50));
		if (ImGui::Begin(glm::to_string(diff).c_str(), nullptr, ImGuiWindowFlags_NoDecoration))
		{
			if (ImGui::Button(label.c_str()))
			{
				spriteData->pose.position += diff;
			}
		}
		ImGui::End();
	}

	void SpriteEditWindow::editorCellFloat(std::string label, float* val, float interval)
	{
		char c[5];
		std::snprintf(c, 5, "%.2f", interval);
		ImGui::DragFloat(label.c_str(), val, 0.01);
		if (ImGui::Button((std::string("-") + c + label).c_str()))
		{
			*val -= interval;
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("+") + c + label).c_str()))
		{
			*val += interval;
		}

		if (ImGui::Button((std::string("|<-") + label).c_str()))
		{
			*val = std::floor(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("->|") + label).c_str()))
		{
			*val = std::ceil(*val);
		}
	}

	void SpriteEditWindow::editorCellAngle(std::string label, float* val, float interval)
	{
		char c[5];
		std::snprintf(c, 5, "%.2f", interval);
		ImGui::DragFloat(label.c_str(), val, 0.1);
		if (ImGui::Button((std::string("-") + c + label).c_str()))
		{
			*val -= interval;
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("+") + c + label).c_str()))
		{
			*val += interval;
		}

		if (ImGui::Button((std::string("|<-") + label).c_str()))
		{
			*val = std::floor(*val);
		}
		ImGui::SameLine();
		if (ImGui::Button((std::string("->|") + label).c_str()))
		{
			*val = std::ceil(*val);
		}
	}

}
