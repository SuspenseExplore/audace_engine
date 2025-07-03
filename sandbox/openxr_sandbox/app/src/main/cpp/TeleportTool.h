#ifndef SANDBOX_OXR_TELEPORTTOOL_H
#define SANDBOX_OXR_TELEPORTTOOL_H

#include "renderer/Sprite.h"

namespace Audace
{
	class Scene;
	class PoseInputEvent;
	class FloatInputEvent;
}

class TeleportTool : public Audace::Sprite {
	Audace::Pose handPose;
	bool primed = false;
	float distance = 0.0;

	public:
	TeleportTool();
	void handlePoseEvent(Audace::PoseInputEvent e);
	void handleSqueezeEvent(Audace::FloatInputEvent e);
	void handleStickMotionEvent(Audace::FloatInputEvent e);
	bool isTeleportReady();
	void reset();

	void renderWorldSpace(Audace::Scene *scene) override;
	void renderViewSpace(Audace::Scene *scene) override;
};
#endif