//
// Created by Suspense on 9/23/2023.
//

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "HmdCamera.h"

const glm::vec3 &HmdCamera::getPosition() {
	XrPosef pose = views[activeView].getViewData().pose;
	return glm::vec3(pose.position.x, pose.position.y, pose.position.z);
}

const glm::mat4 HmdCamera::getViewProjMatrix() {
	XrPosef pose = views[activeView].getViewData().pose;
	XrFovf fov = views[activeView].getViewData().fov;
	glm::mat4 camMat = glm::mat4_cast(glm::quat(pose.orientation.w, pose.orientation.x, pose.orientation.y, pose.orientation.z));
	camMat = glm::transpose(camMat);
	camMat = glm::translate(camMat, -glm::vec3(pose.position.x, pose.position.y, pose.position.z));
	glm::mat4 projMat = glm::frustum(tan(fov.angleLeft) * 0.01f, tan(fov.angleRight) * 0.01f, tan(fov.angleDown) * 0.01f, tan(fov.angleUp) * 0.01f, 0.01f, 100.0f);
	glm::mat4 vpMat = projMat * camMat;
	return vpMat;
}
