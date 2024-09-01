#ifndef _LIB_TIME_H
#define _LIB_TIME_H

#include <stdint.h>
#include "bsp_dwt.h"

#define _time_init(CPU_Freq_mHz) DWT_Init(CPU_Freq_mHz)

#define user_clock(void) DWT_GetTimeline_s(void);   // 获取当前时间，单位为秒

#endif // !_LIB_TIME_H
