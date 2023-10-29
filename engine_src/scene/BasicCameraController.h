#ifndef AU_BASICCAMERACONTROLLER_H
#define AU_BASICCAMERACONTROLLER_H

#include <functional>
#include "ICameraController.h"
#include "ForwardCamera.h"
#include "input/BooleanInputEvent.h"
#include "input/Vec2InputEvent.h"
#include "glm/glm.hpp"

namespace Audace
{
	/**
	 * Provides simple camera controls: right-drag to aim; standard WASD movement; plus Q and Z for moving up and down
	 */
	class BasicCameraController : public ICameraController
	{
		ForwardCamera *camera;

		bool aiming = false;
		glm::vec2 lastMousePos;
		float yaw = 0.0f;
		float pitch = 0.0f;

		float velFactor = 0.01f;
		glm::vec3 velocity = glm::vec3(0);

	public:
		BasicCameraController(ForwardCamera *camera);
		void setVelocityFactor(float v) { velFactor = v; }
		void update() override;

		std::function<void(BooleanInputEvent)> forwardAction;
		std::function<void(BooleanInputEvent)> backwardAction;
		std::function<void(BooleanInputEvent)> leftAction;
		std::function<void(BooleanInputEvent)> rightAction;
		std::function<void(BooleanInputEvent)> upAction;
		std::function<void(BooleanInputEvent)> downAction;
		std::function<void(BooleanInputEvent)> rightMouseAction;
		std::function<void(Vec2InputEvent)> aimAction;
	};
}

#endif