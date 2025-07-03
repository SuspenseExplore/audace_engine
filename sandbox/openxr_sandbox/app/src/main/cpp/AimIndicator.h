//
// Created by Suspense on 5/18/2025.
//

#ifndef OPENXR_SANDBOX_AIMINDICATOR_H
#define OPENXR_SANDBOX_AIMINDICATOR_H

#include "input/PoseInputEvent.h"
#include "renderer/Sprite.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace Audace
{
	class ShaderProgram;
	class Scene;
}

class AimIndicator : public Audace::Sprite {
	Audace::Pose pose;
	glm::vec3 scale = {0.001, 0.001, 1.0};
	glm::vec3 offset = {-0.5, -0.5, -1.0};

public:
	AimIndicator();

	void handlePoseEvent(Audace::PoseInputEvent e);
	void renderWorldSpace(Audace::Scene *scene);

	void renderViewSpace(Audace::Scene *scene) override
	{
		// ImGui::Begin("Left Hand Aim");
		// ImGui::SetWindowPos(ImVec2(600, 800));
		// ImGui::DragFloat3("offset", glm::value_ptr(offset), 0.01, -10.0, 10.0);
		// ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01, 0.0, 10.0);
		// ImGui::End();
	}
};


#endif //OPENXR_SANDBOX_AIMINDICATOR_H
