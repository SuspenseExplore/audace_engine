//
// Created by Suspense on 9/23/2023.
//

#ifndef AU_HMDCAMERA_H
#define AU_HMDCAMERA_H

#include "scene/BaseCamera.h"
#include "OpenxrView.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

/*
 * Abstracts multiple views into a single camera.
 * Each frame before rendering a view, call setActiveView() with the proper index.
 *
 */
class HmdCamera : public Audace::BaseCamera {
protected:
	std::vector<OpenxrView> &views;
	int activeView = -1;

	float velFactor = 0.05f;
	glm::vec3 velocity = glm::vec3(0);

public:
	HmdCamera(std::vector<OpenxrView> &views) : views(views) {}

	void setActiveView(int i) { activeView = i; }

	const glm::vec3 getPosition() override;
	void setVelocityX(float v) { velocity.x = v; }
	void setVelocityY(float v) { velocity.y = v; }
	void update() override
	{
		Audace::BaseCamera::update();
		if (glm::length(velocity) != 0.0) {
			glm::vec4 fwd = glm::vec4(velocity.x, 0.0, -velocity.y, 0);
			glm::vec4 right = glm::vec4(1, 0, 0, 0);
			XrQuaternionf orientation = views[activeView].getViewData().pose.orientation;
			glm::mat4 camMat = glm::mat4_cast(
					glm::quat(orientation.w, orientation.x, orientation.y, orientation.z));
			fwd = camMat * fwd;
			fwd *= velFactor;
			originPos += glm::vec3(fwd.x, fwd.y, fwd.z);
		}
	}

	const glm::mat4 getViewProjMatrix() override;
};

#endif //AU_HMDCAMERA_H
