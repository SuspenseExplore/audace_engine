#ifndef AU_ROTATIONANIMATION_H
#define AU_ROTATIONANIMATION_H

#include <vector>
#include "INodeAnimation.h"
#include "glm/gtc/quaternion.hpp"
#include "util/TimeUtil.h"

namespace Audace
{
	class RotationAnimation : public INodeAnimation
	{
		std::vector<float> frameTimes;
		std::vector<glm::quat> frameStates;
		float lastUpdateTime = -1;
		int lastUpdateFrame = 0;
		float timeFactor = 1;

	public:
		RotationAnimation() {}
		void setFrameTimes(std::vector<float>& times) { frameTimes = times; }
		void setFrameStates(std::vector<glm::quat>& states) { frameStates = states; }
		void setTimeFactor(float f) { timeFactor = f; }

		void start() override
		{
		}
		void update() override;
		int nextFrameIndex()
		{
			return (lastUpdateFrame + 1) % (frameTimes.size());
		}
	};
}

#endif