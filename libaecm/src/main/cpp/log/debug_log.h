//
// Created by User on 29.05.2019.
//

#ifndef AECM_DEBUG_LOG_H
#define AECM_DEBUG_LOG_H

#include <android/log.h>

#define LOGD(tag, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__);
#define LOGE(tag, ...) __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__);

#endif //AECM_DEBUG_LOG_H
