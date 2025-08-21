#include "au_renderer.h"
#include "NodeEditWindow.h"
#include "content/IFileAccess.h"
#include "content/AssetStore.h"
#include "content/Model.h"
#include "editor/SpriteData.h"
#include "renderer/Mesh.h"
#include "renderer/Shapes.h"
#include "renderer/Sprite.h"
#include "renderer/light/PointLight.h"
#include "renderer/light/DirLight.h"
#include "renderer/light/SpotLight.h"
#include "scene/Scene.h"
#include "scene/BaseCamera.h"
#include "scene/graph/SceneGraphNode.h"
#include "imgui.h"
#include "glm/gtx/string_cast.hpp"

namespace Audace
{
	NodeEditWindow::NodeEditWindow(IFileAccess *fileLoader)
	{
		positionMark = new Sprite({Shapes::spherePositions(8, 8)});
		positionMark->getMesh()->setMaterial(AssetStore::simpleBillboardMaterial());
		positionMark->setScale({0.05, 0.05, 0.05});
		jsonGui = new JsonGui(fileLoader, "ui/node_editor.json");
	}

	void NodeEditWindow::setNode(SceneGraphNode *node)
	{
		this->node = node;
		nodeType = node->getNodeType();
		translation = node->getTranslation();
		scale = node->getScale();
		angles = glm::degrees(glm::eulerAngles(node->getRotation()));
		jsonGui->addBinding("Translate", &translation);
		jsonGui->addBinding("Scale", &scale);
		jsonGui->addBinding("Rotate", &angles);
		jsonGui->addBinding("node_type", &nodeType);
		jsonGui->addBinding("Quat value", &quatVal[0]);
		if (nodeType == DIRLIGHT_NODE)
		{
			DirLight *light = reinterpret_cast<DirLight *>(node->getSprite());
			lightColor = light->getColor();
			jsonGui->addBinding("Color", &lightColor);
		}
		else if (nodeType == PTLIGHT_NODE)
		{
			PointLight *light = reinterpret_cast<PointLight *>(node->getSprite());
			lightColor = glm::vec4(light->getColor(), light->getIntensity());
			jsonGui->addBinding("Color", &lightColor);
		}
		else if (nodeType == SPOTLIGHT_NODE)
		{
			SpotLight *light = reinterpret_cast<SpotLight *>(node->getSprite());
			lightColor = glm::vec4(light->getColor(), light->getIntensity());
			spotlightInnerAngle = light->getInnerAngle();
			spotlightOuterAngle = light->getOuterAngle();
			jsonGui->addBinding("Color", &lightColor);
			jsonGui->addBinding("Inner Angle", &spotlightInnerAngle);
			jsonGui->addBinding("Outer Angle", &spotlightOuterAngle);
		}
	}

	void NodeEditWindow::renderWorldSpace(Scene *scene)
	{
		// positionMark->setPosition(node->getPosition());
		// glDisable(GL_DEPTH_TEST);
		// positionMark->renderWorldSpace(scene);
		// glEnable(GL_DEPTH_TEST);
	}

	void NodeEditWindow::renderViewSpace(Scene *scene)
	{
		std::vector<float> intervals = {0.01, 0.1, 1, 5, 10, 15};
		std::vector<float> angleIntervals = {0.01, 0.1, 1, 5, 15, 45, 90};
		static int intervalIndex = 2;
		static int angleIntervalIndex = 6;

		txWidgets(scene);

		jsonGui->render();

		node->setTranslation(translation);
		node->setScale(scale);
		glm::quat q = glm::quat(glm::radians(angles));
		node->setRotation(q);
		std::string s = "[" + std::to_string(q.x) + ", " + std::to_string(q.y) + ", " + std::to_string(q.z) + ", " + std::to_string(q.w) + "]";
		strcpy(quatVal, s.c_str());
		switch (nodeType)
		{
		case DIRLIGHT_NODE:
		{
			DirLight *light = reinterpret_cast<DirLight *>(node->getSprite());
			light->setColor(lightColor);
			break;
		}

		case PTLIGHT_NODE:
		{
			PointLight *light = reinterpret_cast<PointLight *>(node->getSprite());
			light->setColor(glm::vec3(lightColor));
			light->setIntensity(lightColor.a);
			break;
		}

		case SPOTLIGHT_NODE:
		{
			SpotLight *light = reinterpret_cast<SpotLight *>(node->getSprite());
			light->setColor(glm::vec3(lightColor));
			light->setIntensity(lightColor.a);
			light->setInnerAngle(spotlightInnerAngle);
			light->setOuterAngle(spotlightOuterAngle);
			break;
		}
		}
	}

	void NodeEditWindow::txWidgets(Scene *scene)
	{
		ImGui::PushID("tx_widgets");
		moveWidgetButton(scene, glm::vec3(-1, 0, 0), "-X");
		moveWidgetButton(scene, glm::vec3(1, 0, 0), "+X");
		moveWidgetButton(scene, glm::vec3(0, -1, 0), "-Y");
		moveWidgetButton(scene, glm::vec3(0, 1, 0), "+Y");
		moveWidgetButton(scene, glm::vec3(0, 0, -1), "-Z");
		moveWidgetButton(scene, glm::vec3(0, 0, 1), "+Z");
		ImGui::PopID();
	}

	void NodeEditWindow::moveWidgetButton(Scene *scene, glm::vec3 diff, std::string label)
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
}
