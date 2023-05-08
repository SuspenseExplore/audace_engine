//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_AUDACE_COMMON_H
#define AUDACE_AUDACE_COMMON_H

#include <jni.h>
#include <android/log.h>

#define LOGD(...) ((void) __android_log_print(ANDROID_LOG_DEBUG, "AUDACE", __VA_ARGS__))
#define LOGE(...) ((void) __android_log_print(ANDROID_LOG_ERROR, "AUDACE", __VA_ARGS__))

#endif //AUDACE_AUDACE_COMMON_H
