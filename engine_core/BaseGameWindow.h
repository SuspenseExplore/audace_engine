#ifndef AU_BASEGAMEWINDOW_H
#define AU_BASEGAMEWINDOW_H

namespace Audace {
	class BaseGameWindow {
		protected:
		bool initialized = false;

		public:
		virtual void processEvents() {}

		bool isInitialized() {return initialized;}
		virtual bool shouldClose() {return true;}
		virtual bool open() {return false;}
		virtual void close() {}

		virtual void beginFrame() {}
		virtual void endFrame() {}
	};
}

#endif