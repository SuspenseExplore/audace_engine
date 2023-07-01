//
// Created by Suspense on 6/29/2023.
//

#include "PoseCameraController.h"

namespace Audace {
	PoseCameraController::PoseCameraController(ForwardCamera *camera)
			: camera(camera) {
		camera->setController(this);
		poseAction = [this](PoseInputEvent event) {
			pose = event.state;
		};
	}

	void PoseCameraController::update() {
		camera->setPosition(-pose.position);
		camera->setOrientation(-pose.orientation);
	}
}