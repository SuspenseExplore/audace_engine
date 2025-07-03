#include "SpriteData.h"
#include "renderer/Sprite.h"

namespace Audace
{
	void SpriteData::syncToSprite()
	{
		sprite->setPosition(pose.position);
		sprite->setOrientation(pose.orientation);
		sprite->setScale(scale);
	}
}