#ifndef AUDACE_H
#define AUDACE_H

#include "Application.h"
#include "BaseGameWindow.h"
#include "au_platform.h"

namespace Audace {
	extern Application* createApp(BaseGameWindow *window);
}

#endif