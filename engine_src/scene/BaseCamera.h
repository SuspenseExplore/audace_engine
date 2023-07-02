#ifndef AU_BASECAMERA_H
#define AU_BASECAMERA_H

#include "glm/glm.hpp"
#include "ICameraController.h"

namespace Audace {
	class BaseCamera {
	protected:
		ICameraController *controller = nullptr;
		glm::mat4 projMat;

	public:
		virtual const glm::vec3 &getPosition() = 0;

		virtual const glm::mat4 getViewProjMatrix() = 0;

		void setProjectionMatrix(glm::mat4 mat) { projMat = mat; }

		void setController(ICameraController *ctl) { controller = ctl; }

		void update() {
			if (controller != nullptr) {
				controller->update();
			}
		}
	};
}

#endif