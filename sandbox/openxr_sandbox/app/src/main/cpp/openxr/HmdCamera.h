//
// Created by Suspense on 9/23/2023.
//

#ifndef AU_HMDCAMERA_H
#define AU_HMDCAMERA_H

#include "scene/BaseCamera.h"
#include "OpenxrView.h"

/*
 * Abstracts multiple views into a single camera.
 * Each frame before rendering a view, call setActiveView() with the proper index.
 *
 */
class HmdCamera : public Audace::BaseCamera {
protected:
	std::vector<OpenxrView> &views;
	int activeView = -1;

public:
	HmdCamera(std::vector<OpenxrView> &views) : views(views) {}

	void setActiveView(int i) { activeView = i; }

	const glm::vec3 getPosition() override;

	const glm::mat4 getViewProjMatrix() override;
};

#endif //AU_HMDCAMERA_H
