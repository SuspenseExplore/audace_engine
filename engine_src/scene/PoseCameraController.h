//
// Created by Suspense on 6/29/2023.
//

#ifndef AU_POSECAMERACONTROLLER_H
#define AU_POSECAMERACONTROLLER_H

#include <functional>
#include "ICameraController.h"

namespace Audace {
	class ForwardCamera;
	class PoseInputEvent;
	
	class PoseCameraController : public ICameraController {
		ForwardCamera *camera;
		Pose pose;

	public:
		PoseCameraController(ForwardCamera *camera);
		void update() override;

		std::function<void(PoseInputEvent)> poseAction;
	};
}

#endif //AU_POSECAMERACONTROLLER_H
