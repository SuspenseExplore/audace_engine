#ifndef AU_BASECAMERA_H
#define AU_BASECAMERA_H

#include "glm/glm.hpp"
#include "ICameraController.h"

namespace Audace {
	class BaseCamera {
	protected:
		ICameraController* controller = nullptr;
		glm::vec3 originPos = { 0, 0, 0 }; // used by VR for teleporting
		glm::mat4 projMat;

	public:
		virtual const glm::vec3 getPosition() = 0;

		virtual const glm::mat4 getViewProjMatrix() = 0;
		virtual const glm::vec2 getViewSize() = 0;

		void setProjectionMatrix(glm::mat4 mat) { projMat = mat; }

		void setController(ICameraController* ctl) { controller = ctl; }

		void setOriginPos(glm::vec3 pos) { originPos = pos; }
		glm::vec3 getOriginPos() { return originPos; }

		virtual void update() {
			if (controller != nullptr) {
				controller->update();
			}
		}

		/*
		Project a world-space position to clip space using this camera's view and projection transforms.
		The returned point should be clipped to (-1, 1) on all 3 axes.
		*/
		glm::vec3 projectClipSpace(glm::vec3 worldPos)
		{
			glm::mat4 m = getViewProjMatrix();
			glm::vec4 p = m * glm::vec4(worldPos, 1.0);
			p = p / p.w;
			return glm::vec3(p);
		}

		glm::ivec2 projectScreenSpace(glm::vec3 worldPos)
		{
			glm::vec3 clip = projectClipSpace(worldPos);
			glm::vec2 size = getViewSize();
			double x = (clip.x + 1.0) * 0.5 * size.x;
			double y = (1.0 - clip.y) * 0.5 * size.y;
			return glm::ivec2(x, y);
		}
	};
}

#endif