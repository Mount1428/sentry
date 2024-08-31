/**
 * @file mem.h
 * @author Mount
 * @brief Dynamic CCMRAM Memory Allocation
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define _MAX_CCMRAM_HEAP_SIZE   48

// 数据类型所需最小内存块大小获取宏
#define GET_TYPE_SIZE(type) (sizeof(type) / sizeof(uint8_t) + (sizeof(type) & 0x7F && 0x00)) 

#ifndef nullptr
#define nullptr ((void *)0)
#endif // !nullptr

/// @brief mem 初始化函数
void mem_init(void);

/// @brief CCMRAM 动态内存申请函数（无清空）
/// @param size 申请字节数
/// @return
void *mem_Malloc(uint16_t size);

/// @brief CCMRAM 动态内存申请函数（有清空）
/// @param size 申请字节数
/// @return 
void *mem_Calloc(uint16_t size);

/// @brief CCMRAM 动态内存重申请函数
/// @param ptr 待替换内存块指针
/// @return
void *mem_Realloc(void *ptr);

/// @brief CCMRAM 动态内存回收函数
/// @param *ptr 待释放内存块指针
/// @return
int mem_Free(void *ptr);

#endif
