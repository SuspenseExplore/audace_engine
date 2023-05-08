//
// Created by Suspense on 3/2/2023.
//

#ifndef AUDACE_OPENXR_COMMON_H
#define AUDACE_OPENXR_COMMON_H

#include "audace_common.h"
#include <EGL/egl.h>

#define XR_USE_GRAPHICS_API_OPENGL_ES
#define XR_USE_PLATFORM_ANDROID

#include "openxr.h"
#include "openxr_platform.h"
#include "openxr_platform_defines.h"
#include "openxr_reflection.h"

#define XR_LOG_ERROR(_name, _call) ({XrResult _r = _call; if (_r != XR_SUCCESS) LOGE("%s failed: %d", _name, _r);})
#define XR_ERROR_BAIL(_name, _call) ({XrResult _r = _call; if (_r != XR_SUCCESS) {LOGE("%s failed: %d", _name, _r); return false;}})

#endif //AUDACE_OPENXR_COMMON_H
