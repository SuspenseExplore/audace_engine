//
// Created by Suspense on 9/23/2023.
//

#ifndef AU_HMDCAMERA_H
#define AU_HMDCAMERA_H

#include <vector>
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
	HmdCamera(std::vector<OpenxrView> &views);

	void setActiveView(int i) { activeView = i; }

	const glm::vec3 getPosition() override;
	void setVelocityX(float v) { velocity.x = v; }
	void setVelocityY(float v) { velocity.y = v; }
	void update() override;

	const glm::vec2 getViewSize() override;
	const glm::mat4 getViewProjMatrix() override;
};

#endif //AU_HMDCAMERA_H
