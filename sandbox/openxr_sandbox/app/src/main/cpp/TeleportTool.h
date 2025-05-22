#ifndef SANDBOX_OXR_TELEPORTTOOL_H
#define SANDBOX_OXR_TELEPORTTOOL_H

#include "input/PoseInputEvent.h"
#include "input/FloatInputEvent.h"
#include "renderer/Sprite.h"
#include "scene/Scene.h"
#include "renderer/Shapes.h"

namespace au = Audace;

class TeleportTool : public au::Sprite {
	au::Pose handPose;
	bool primed = false;
	float distance = 0.0;

	public:
	TeleportTool() : au::Sprite({au::Shapes::spherePositions(16, 16)})
	{
		setScale({0.01, 0.01, 0.01});
	}

	void handlePoseEvent(au::PoseInputEvent e);
	void handleSqueezeEvent(au::FloatInputEvent e);
	void handleStickMotionEvent(au::FloatInputEvent e);
	bool isTeleportReady();
	void reset();

	void renderWorldSpace(au::Scene *scene) override;
	void renderViewSpace(au::Scene *scene) override;
};
#endif