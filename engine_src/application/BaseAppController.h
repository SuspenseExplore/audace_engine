#ifndef AU_BASEAPPCONTROLLER_H
#define AU_BASEAPPCONTROLLER_H

namespace Audace {
	class BaseAppController {
		public:
		virtual void setScene(int nextScene) = 0;
	};
}

#endif