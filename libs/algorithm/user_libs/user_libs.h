/**
 * @file user_libs.h
 * @author Zhou Mingfeng
 * @brief
 * @version 1.0
 * @date 2024-09-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _USER_LIBS_H
#define _USER_LIBS_H

#include <stdint.h>

#ifndef user_malloc
#if defined(_CMSIS_OS_H)
#define user_malloc pvPortMalloc
#elif defined(MEM_H)
#define user_malloc mem_Malloc
#endif
#endif // !user_malloc

/* boolean type definitions */
#ifndef bool
#define bool _Bool
#endif // !bool

#ifndef true
#define true ((bool)1)
#endif // !true

#ifndef false
#define false ((bool)0)
#endif // !false

/* math relevant */
/* radian coefficient */
#ifndef RADIAN_COEF
#define RADIAN_COEF 57.295779513f
#endif

/* circumference ratio */
#ifndef PI
#define PI 3.14159265354f
#endif

// 数值限幅
#define VAL_LIMIT_RANGE(val, min, max) \
    do                                 \
    {                                  \
        if ((val) < (min))             \
        {                              \
            (val) = (min);             \
        }                              \
        else if ((val) > (max))        \
        {                              \
            (val) = (max);             \
        }                              \
    } while (0);

/// @brief 快速开方（牛顿迭代法）
/// @param x 
/// @return 
float Sqrt(float x);

/// @brief 快速倒数开方
/// @param x 
/// @return 1 / Sqrt(x)
float invSqrt(float x);

#endif // !_USER_LIBS_H
