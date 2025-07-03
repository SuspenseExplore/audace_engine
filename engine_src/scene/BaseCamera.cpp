#include "BaseCamera.h"
#include "ICameraController.h"

namespace Audace
{

	void BaseCamera::setProjectionMatrix(glm::mat4 mat)
	{
		projMat = mat;
	}

	void BaseCamera::setController(ICameraController* ctl)
	{
		controller = ctl;
	}

	void BaseCamera::setOriginPos(glm::vec3 pos)
	{
		originPos = pos;
	}

	glm::vec3 BaseCamera::getOriginPos()
	{
		return originPos;
	}

	void BaseCamera::update() {
		if (controller != nullptr) {
			controller->update();
		}
	}

	glm::vec3 BaseCamera::projectClipSpace(glm::vec3 worldPos)
	{
		glm::mat4 m = getViewProjMatrix();
		glm::vec4 p = m * glm::vec4(worldPos, 1.0);
		p = p / p.w;
		return glm::vec3(p);
	}

	glm::ivec2 BaseCamera::projectScreenSpace(glm::vec3 worldPos)
	{
		glm::vec3 clip = projectClipSpace(worldPos);
		glm::vec2 size = getViewSize();
		double x = (clip.x + 1.0) * 0.5 * size.x;
		double y = (1.0 - clip.y) * 0.5 * size.y;
		return glm::ivec2(x, y);
	}

}