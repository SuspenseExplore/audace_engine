#include "BasicCameraController.h"
#include "KeyboardManager.h"

namespace Audace
{
	BasicCameraController::BasicCameraController(ForwardCamera *camera)
	{
		this->camera = camera;
		forwardAction = [this](BooleanInputEvent event)
		{
			velocity.y = event.state ? velFactor : 0;
		};
		backwardAction = [this](BooleanInputEvent event)
		{
			velocity.y = event.state ? -velFactor : 0;
		};
		leftAction = [this](BooleanInputEvent event)
		{
			velocity.x = event.state ? -velFactor : 0;
		};
		rightAction = [this](BooleanInputEvent event)
		{
			velocity.x = event.state ? velFactor : 0;
		};
		upAction = [this](BooleanInputEvent event)
		{
			velocity.z = event.state ? velFactor : 0;
		};
		downAction = [this](BooleanInputEvent event)
		{
			velocity.z = event.state ? -velFactor : 0;
		};
		rightMouseAction = [this](BooleanInputEvent event)
		{
			aiming = event.state;
		};
		aimAction = [this](Vec2InputEvent event)
		{
			if (event.changed && aiming)
			{
				pitch += (event.state.y - lastMousePos.y) * 0.005f;
				yaw += (event.state.x - lastMousePos.x) * 0.005f;
			}
			lastMousePos = event.state;
		};
	}

	void BasicCameraController::update()
	{
		camera->setOrientation(pitch, 0, yaw);
		camera->move(velocity);
	}
}