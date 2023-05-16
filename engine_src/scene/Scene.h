#ifndef AU_SCENE_H
#define AU_SCENE_H

namespace Audace {
	class Scene {
		public:
		virtual ~Scene() {}
		virtual void loadAssets() = 0;
		virtual void render() = 0;
		virtual void disposeAssets() = 0;
	};
}

#endif