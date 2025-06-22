#ifndef AU_INODEANIMATION_H
#define AU_INODEANIMATION_H

namespace Audace
{
	class SceneGraphNode;

	class INodeAnimation
	{
	protected:
		SceneGraphNode* node;

	public:
		INodeAnimation() {}

		void setNode(SceneGraphNode* n) { node = n; }
		virtual void start() = 0;
		virtual void update() = 0;
	};
}

#endif