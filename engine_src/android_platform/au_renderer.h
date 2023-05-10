//
// Created by Suspense on 5/6/2023.
//

#ifndef AU_RENDERER_H
#define AU_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl32.h>

#include "AuLogger.h"

#define AU_RENDERER_LOG_CRITICAL(...) Audace::AuLogger::getEngineLogger()->getLogger()->critical(__VA_ARGS__)
#define AU_RENDERER_LOG_ERROR(...)    Audace::AuLogger::getEngineLogger()->getLogger()->error(__VA_ARGS__)
#define AU_RENDERER_LOG_WARN(...)     Audace::AuLogger::getEngineLogger()->getLogger()->warn(__VA_ARGS__)
#define AU_RENDERER_LOG_INFO(...)     Audace::AuLogger::getEngineLogger()->getLogger()->info(__VA_ARGS__)
#define AU_RENDERER_LOG_DEBUG(...)    Audace::AuLogger::getEngineLogger()->getLogger()->debug(__VA_ARGS__)
#define AU_RENDERER_LOG_TRACE(...)    Audace::AuLogger::getEngineLogger()->getLogger()->trace(__VA_ARGS__)

#endif //AU_RENDERER_H
