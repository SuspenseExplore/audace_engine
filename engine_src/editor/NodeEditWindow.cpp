#include "au_renderer.h"
#include "NodeEditWindow.h"
#include "content/AssetStore.h"
#include "content/Model.h"
#include "editor/SpriteData.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "scene/Scene.h"
#include "scene/BaseCamera.h"
#include "scene/graph/SceneGraphNode.h"
#include "imgui.h"
#include "glm/gtx/string_cast.hpp"

namespace Audace
{
	NodeEditWindow::NodeEditWindow()
	{
		positionMark = new Sprite({ Shapes::spherePositions(8, 8) });
		positionMark->getMesh()->setMaterial(AssetStore::simpleBillboardMaterial());
		positionMark->setScale({ 0.05, 0.05, 0.05 });
	}

	void NodeEditWindow::setNode(SceneGraphNode* node)
	{
		this->node = node;
		translation = node->getTranslation();
		scale = node->getScale();
		angles = glm::degrees(glm::eulerAngles(node->getRotation()));
	}

	void NodeEditWindow::renderWorldSpace(Scene* scene)
	{
		// positionMark->setPosition(node->getPosition());
		// glDisable(GL_DEPTH_TEST);
		// positionMark->renderWorldSpace(scene);
		// glEnable(GL_DEPTH_TEST);
	}

	void NodeEditWindow::renderViewSpace(Scene* scene)
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
					editorCellFloat("##Xpos", &translation.x, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Ypos", &translation.y, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Zpos", &translation.z, intervals[intervalIndex]);

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
					editorCellFloat("##Xscale", &scale.x, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Yscale", &scale.y, intervals[intervalIndex]);
					ImGui::TableNextColumn();
					editorCellFloat("##Zscale", &scale.z, intervals[intervalIndex]);

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
					editorCellAngle("##Xangle", &angles.x, angleIntervals[angleIntervalIndex]);
					ImGui::TableNextColumn();
					editorCellAngle("##Yangle", &angles.y, angleIntervals[angleIntervalIndex]);
					ImGui::TableNextColumn();
					editorCellAngle("##Zangle", &angles.z, angleIntervals[angleIntervalIndex]);

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
		node->setTranslation(translation);
		node->setScale(scale);
		node->setRotation(glm::quat(glm::radians(angles)));
	}

	void NodeEditWindow::txWidgets(Scene* scene)
	{
		ImGui::PushID("tx_widgets");
		translateButton(scene, glm::vec3(-1, 0, 0), "-X");
		translateButton(scene, glm::vec3(1, 0, 0), "+X");
		translateButton(scene, glm::vec3(0, -1, 0), "-Y");
		translateButton(scene, glm::vec3(0, 1, 0), "+Y");
		translateButton(scene, glm::vec3(0, 0, -1), "-Z");
		translateButton(scene, glm::vec3(0, 0, 1), "+Z");
		ImGui::PopID();
	}

	void NodeEditWindow::translateButton(Scene* scene, glm::vec3 diff, std::string label)
	{
		glm::mat4 tx = node->getLocalTransform();
		glm::vec3 worldPos = glm::vec3(tx * glm::vec4(diff, 1.0));
		glm::ivec2 p = scene->getCamera()->projectScreenSpace(worldPos);
		ImGui::SetNextWindowPos(ImVec2(p.x, p.y));
		ImGui::SetNextWindowSize(ImVec2(50, 50));
		if (ImGui::Begin(glm::to_string(diff).c_str(), nullptr, ImGuiWindowFlags_NoDecoration))
		{
			if (ImGui::Button(label.c_str()))
			{
				glm::mat4 m = glm::mat4_cast(node->getRotation()) * glm::scale(glm::mat4(1.0), node->getScale());
				translation += glm::vec3(m * glm::vec4(diff, 0.0));
			}
		}
		ImGui::End();
	}

	void NodeEditWindow::editorCellFloat(std::string label, float* val, float interval)
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

	void NodeEditWindow::editorCellAngle(std::string label, float* val, float interval)
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
