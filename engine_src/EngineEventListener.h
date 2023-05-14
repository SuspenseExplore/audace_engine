#ifndef AU_ENGINEEVENTLISTENER_H
#define AU_ENGINEEVENTLISTENER_H

namespace Audace {
	class EngineEventListener {
		public:
		virtual void windowInitialized() = 0;
	};
}

#endif