#ifndef NESEMU_LOGUTIL_H
#define NESEMU_LOGUTIL_H

//#define LOG_LEVEL_DEBUG
//#define LOG_LEVEL_INFO

#ifdef LOG_LEVEL_INFO
#define L_INFO(c) c
#else
#define L_INFO(c) do {} while(0)
#endif

#ifdef LOG_LEVEL_DEBUG
#define L_DEBUG(c) c
#else
#define L_DEBUG(c) do {} while (0)
#endif

#ifdef LOG_LEVEL_ERROR
#define L_ERROR(c) c
#else
#define L_ERROR(c) do {} while (0)
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || defined(LOG_LEVEL_ERROR)
#include <iostream>
#endif


#endif //NESEMU_LOGUTIL_H
