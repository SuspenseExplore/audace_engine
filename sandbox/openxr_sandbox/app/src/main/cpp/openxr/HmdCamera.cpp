//
// Created by Suspense on 9/23/2023.
//

#include "HmdCamera.h"

HmdCamera::HmdCamera(std::vector<OpenxrView> &views)
		: views(views) {

}

const glm::vec3 HmdCamera::getPosition() {
	XrPosef pose = views[activeView].getViewData().pose;
	return glm::vec3(pose.position.x, pose.position.y, pose.position.z);
}

const glm::vec2 HmdCamera::getViewSize() {
	return glm::vec2(views[activeView].getWidth(), views[activeView].getHeight());
}

const glm::mat4 HmdCamera::getViewProjMatrix() {
	XrPosef pose = views[activeView].getViewData().pose;
	XrFovf fov = views[activeView].getViewData().fov;
	glm::mat4 camMat = glm::mat4_cast(
			glm::quat(pose.orientation.w, pose.orientation.x, pose.orientation.y,
					  pose.orientation.z));
	camMat = glm::transpose(camMat);
	camMat = glm::translate(camMat, -(originPos + glm::vec3(pose.position.x, pose.position.y,
															pose.position.z)));
	glm::mat4 projMat = glm::frustum(tan(fov.angleLeft) * 0.01f, tan(fov.angleRight) * 0.01f,
									 tan(fov.angleDown) * 0.01f, tan(fov.angleUp) * 0.01f, 0.01f,
									 1000.0f);
	glm::mat4 vpMat = projMat * camMat;
	return vpMat;
}

void HmdCamera::update() {
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
