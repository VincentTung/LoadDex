//


#ifndef LOADDEX_LOG_H
#define LOADDEX_LOG_H


#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif

#define  LOG_TAG    "dlfcn_test"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
}
#endif




#endif //LOADDEX_LOG_H
