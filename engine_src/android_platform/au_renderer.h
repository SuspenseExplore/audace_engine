//
// Created by Suspense on 5/6/2023.
//

#ifndef AU_RENDERER_H
#define AU_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl32.h>

#include "AuLogger.h"

#define AU_RENDERER_LOG_CRITICAL(...) Audace::AuLogger::getRendererLogger()->getLogger()->critical(__VA_ARGS__)
#define AU_RENDERER_LOG_ERROR(...)    Audace::AuLogger::getRendererLogger()->getLogger()->error(__VA_ARGS__)
#define AU_RENDERER_LOG_WARN(...)     Audace::AuLogger::getRendererLogger()->getLogger()->warn(__VA_ARGS__)
#define AU_RENDERER_LOG_INFO(...)     Audace::AuLogger::getRendererLogger()->getLogger()->info(__VA_ARGS__)
#define AU_RENDERER_LOG_DEBUG(...)    Audace::AuLogger::getRendererLogger()->getLogger()->debug(__VA_ARGS__)
#define AU_RENDERER_LOG_TRACE(...)    Audace::AuLogger::getRendererLogger()->getLogger()->trace(__VA_ARGS__)

#define AU_CHECK_GL_ERRORS()\
{\
    GLenum _ERR = GL_NO_ERROR;\
    while ((_ERR = glGetError()) != GL_NO_ERROR)\
    {\
        AU_RENDERER_LOG_ERROR("GL error 0x{:x} at line {} in {}", _ERR, __LINE__, __FILE__);\
    }\
}

#endif //AU_RENDERER_H
