#include "TeleportTool.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "input/PoseInputEvent.h"
#include "input/FloatInputEvent.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Shapes.h"
#include "renderer/Mesh.h"
#include "renderer/material/BaseMaterial.h"
#include "scene/Scene.h"
#include "scene/BaseCamera.h"

TeleportTool::TeleportTool()
		: Audace::Sprite({Audace::Shapes::spherePositions(16, 16)}) {
	setScale({0.01, 0.01, 0.01});
}

void TeleportTool::handlePoseEvent(Audace::PoseInputEvent e) {
	if (e.changed) {
		handPose = e.state;
	}
}

void TeleportTool::handleSqueezeEvent(Audace::FloatInputEvent e) {
	if (!primed && e.state > 0.5 && distance > 0.0) {
		primed = true;
	}
}

void TeleportTool::handleStickMotionEvent(Audace::FloatInputEvent e) {
	if (!primed) {
		distance = e.state * 5.0;
	}
}

bool TeleportTool::isTeleportReady() {
	return primed;
}

void TeleportTool::reset() {
	distance = 0.0;
	primed = false;
}

void TeleportTool::renderWorldSpace(Audace::Scene *scene) {
	getMesh()->getMaterial()->getShader()->bind();
	setPosition(handPose.position + scene->getCamera()->getOriginPos());
	setOrientation(handPose.orientation);
	if (distance > 0.5) {
		glm::vec4 v = {0.0, 0.0, -1.0, 0.0};
		v = v * glm::transpose(glm::mat4_cast(handPose.orientation));
		v *= distance;
		setPosition(handPose.position + scene->getCamera()->getOriginPos() + glm::vec3(v));
		Audace::Sprite::renderWorldSpace(scene);
	}
}

void TeleportTool::renderViewSpace(Audace::Scene *scene) {
	//	ImGui::Begin("Teleport Tool");
	//	ImGui::DragFloat3("offset", glm::value_ptr(offset), 0.01, -10.0, 10.0);
	//	ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01, 0.0, 10.0);
	//	ImGui::End();
}
