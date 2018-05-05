#ifndef HEADER_ANDROID_LOG_H
#define HEADER_ANDROID_LOG_H

#define DEBUG 1

#if DEBUG
#include "log.h"
#  define  LOGI(x,y...)  __android_log_print(ANDROID_LOG_INFO,x,y)
#  define  LOGE(x,y...)  __android_log_print(ANDROID_LOG_ERROR,x,y)
#  define  LOGV(x,y...)  __android_log_print(ANDROID_LOG_VERBOSE,x,y)
#  define  LOGD(x,y...)  __android_log_print(ANDROID_LOG_DEBUG,x,y)
#else
#  define  LOGI(...)  do {} while (0)
#  define  LOGE(...)  do {} while (0)
#  define  LOGV(...)  do {} while (0)
#  define  LOGD(...)  do {} while (0)
#endif

#endif
