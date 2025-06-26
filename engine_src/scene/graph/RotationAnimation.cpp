#include "RotationAnimation.h"
#include "scene/graph/SceneGraphNode.h"

namespace Audace
{
	void RotationAnimation::update()
	{
		if (lastUpdateTime < 0)
		{
			lastUpdateTime = TimeUtil::currentTimeMillis() / 1000.0 * timeFactor;
			return;
		}
		double currentTime = TimeUtil::currentTimeMillis() / 1000.0 * timeFactor;
		double elapsedTime = currentTime - lastUpdateTime;
		int nextIndex = nextFrameIndex();
		if (frameTimes[lastUpdateFrame] + elapsedTime > frameTimes[nextIndex])
		{
			elapsedTime -= (frameTimes[nextIndex] - frameTimes[lastUpdateFrame]);
			lastUpdateFrame = nextIndex;
			nextIndex = nextFrameIndex();
			lastUpdateTime = currentTime;
		}
		float c = frameTimes[lastUpdateFrame] + elapsedTime;
		float t1 = frameTimes[lastUpdateFrame];
		float t2 = frameTimes[nextIndex];
		float i = (c - t1) / (t2 - t1);
		glm::quat q = glm::slerp(frameStates[lastUpdateFrame], frameStates[nextIndex], i);
		// AU_ENGINE_LOG_DEBUG("{},{},{},{}", q.x, q.y, q.z, q.w);
		node->setRotation(q);
	}
}