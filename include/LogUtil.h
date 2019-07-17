#ifndef NESEMU_LOGUTIL_H
#define NESEMU_LOGUTIL_H

#ifdef LOG_ENABLED
#define L_INFO(c) c
#else
#define L_INFO(c) do {} while(0)
#endif



#endif //NESEMU_LOGUTIL_H
