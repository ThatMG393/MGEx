#pragma once

#include <jni.h>
#include <android/log.h>
#include "obfuscate.h"

#define LOG_TAG OBFUSCATE("MGexecutor")

#define LOGD(...) ((void)__android_log_print(3, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(6, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(4, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(5, LOG_TAG, __VA_ARGS__))
