//
// Created by Suspense on 3/31/2023.
//

#ifndef AU_PLATFORM_H
#define AU_PLATFORM_H

#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include "AuLogger.h"

#define CHECK_GL_ERROR(__CALL, __NAME) \
	{GLuint __err = glGetError(); \
	if (__err != GL_NO_ERROR) AU_ENGINE_LOG_ERROR("GL error in {}: {}", __NAME, __err);}

#endif //AU_PLATFORM_H
