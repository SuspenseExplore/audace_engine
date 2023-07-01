#ifndef AU_BASEAPPCONTROLLER_H
#define AU_BASEAPPCONTROLLER_H

namespace Audace {
	class BaseAppController {
		public:
		virtual void setScene(int nextScene) = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
	};
}

#endif