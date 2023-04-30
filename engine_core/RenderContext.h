#ifndef AU_RENDERCONTEXT_H
#define AU_RENDERCONTEXT_H

#include "AuLogger.h"

void printGlErrors(char *fnName);

#define CALL_GL(__NAME, ...) \
	__NAME(__VA_ARGS__); \
	printGlErrors(#__NAME);

namespace Audace {
	class RenderContext {
		
		public:
		void render();
	};
}

#endif