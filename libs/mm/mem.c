#include "mem.h"
#include "cmsis_gcc.h"
#include <string.h>

#pragma pack(1)     // 1字节对齐

__attribute__((section(".ccmram"))) static uint8_t gCCMRAM[_MAX_CCMRAM_HEAP_SIZE * 1024];
static uint32_t idx = 0;

typedef struct _mem {
    uint16_t id;        // 内存块id
    enum {MEM_UNUSED = 0, MEM_USED} is_used;    // 当前内存块使用标记
    uint16_t length;    // 内存块长度
    struct _mem *next;   // 下一内存块位置
    uint8_t *addr;      // 当前内存块实际数据储存地址
} _mem_list_s;

#define _LIST_HEAD_SIZE (sizeof(_mem_list_s))       // 链表节点头大小

#pragma pack()

/// @brief 合并未使用内存函数
/// @param  
inline static void
_mem_merge(void) {
    _mem_list_s *ptr = (_mem_list_s *)gCCMRAM;
    while (ptr->next != nullptr) {
        // 相邻两个链表节点都为UNUSED时进行合并
        if (!(ptr->is_used & ptr->next->is_used)) {
            _mem_list_s *temp_p = ptr->next;
            ptr->next = temp_p->next;
            ptr->length += temp_p->length + _LIST_HEAD_SIZE;
        }
    }
}

/// @brief 内存分配失败错误处理函数
/// @param  
__attribute__((weak)) void
Error_Mem_Get(void) {
    return;
}

void 
mem_init(void) {
    if (!idx) {
        _mem_list_s head = {
            .id = idx++,
            .is_used = MEM_UNUSED,
            .length = _MAX_CCMRAM_HEAP_SIZE * 1024 - _LIST_HEAD_SIZE,
            .next = nullptr,
            .addr = gCCMRAM + head.length
        };

        memcpy(gCCMRAM, &head, _LIST_HEAD_SIZE);
    }
}

void *
mem_Malloc(uint16_t size) {
    if (!idx) {
        return nullptr;
    }
    
    __disable_irq();    // 关闭中断，防止被打断

    _mem_list_s *_list_ptr = (_mem_list_s *)gCCMRAM;
    do {
        if (_list_ptr->length >= size + _LIST_HEAD_SIZE) {
            uint8_t *addr = ((uint8_t *)_list_ptr + _LIST_HEAD_SIZE + size);
            _mem_list_s node = {
                .id = idx++,
                .is_used = MEM_UNUSED,
                .addr = addr + _LIST_HEAD_SIZE,
                .length = _list_ptr->length - _LIST_HEAD_SIZE - size,
                .next = _list_ptr->next
            };
            memcpy(addr, &node, _LIST_HEAD_SIZE);

            _list_ptr->next = (_mem_list_s *)addr;
            _list_ptr->is_used = MEM_USED;
            _list_ptr->length = size;

            break;
        }

    } while ((_list_ptr = _list_ptr->next) != nullptr);

    __enable_irq();     // 恢复中断

    if (_list_ptr != nullptr) {
        return (void *)_list_ptr->addr;
    } else {
        Error_Mem_Get();
        return nullptr;
    }
}

void *
mem_Calloc(uint16_t size) {
    void *ptr = mem_Malloc(size);
    if (ptr != nullptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void *
mem_Realloc(void *ptr) {
    _mem_list_s *_ptr = (_mem_list_s *)((uint8_t *)ptr - _LIST_HEAD_SIZE);
    void *_re_ptr = mem_Malloc(_ptr->length);
    if (_re_ptr != nullptr) {
        memcpy(_re_ptr, ptr, _ptr->length);
        return _re_ptr;
    }
    return ptr;
}

int 
mem_Free(void *ptr) {
    _mem_list_s *_ptr = (_mem_list_s *)((uint8_t *)ptr - _LIST_HEAD_SIZE);
    if ((void *)_ptr >= (void *)gCCMRAM && _ptr->id <= idx) {
        _ptr->is_used = MEM_UNUSED;
        _mem_merge();

        return 0;
    }
    return -1;
}
