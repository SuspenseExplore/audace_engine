#ifndef AU_BASECAMERA_H
#define AU_BASECAMERA_H

#include "glm/glm.hpp"

namespace Audace {
	class ICameraController;

	class BaseCamera {
	protected:
		ICameraController* controller = nullptr;
		glm::vec3 originPos = { 0, 0, 0 }; // used by VR for teleporting
		glm::mat4 projMat;

	public:
		virtual const glm::vec3 getPosition() = 0;

		virtual const glm::mat4 getViewProjMatrix() = 0;
		virtual const glm::vec2 getViewSize() = 0;

		void setProjectionMatrix(glm::mat4 mat);

		void setController(ICameraController* ctl);

		void setOriginPos(glm::vec3 pos);
		glm::vec3 getOriginPos();

		virtual void update();

		/*
		Project a world-space position to clip space using this camera's view and projection transforms.
		The returned point should be clipped to (-1, 1) on all 3 axes.
		*/
		glm::vec3 projectClipSpace(glm::vec3 worldPos);

		glm::ivec2 projectScreenSpace(glm::vec3 worldPos);
	};
}

#endif