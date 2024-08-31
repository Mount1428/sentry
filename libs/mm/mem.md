# mem

提供对 CCMRAM 部分内存的动态内存申请接口

## 使用说明

同 libc 库动态内存使用方法

## 代码结构

## 外部接口

```c

void mem_init(void);
void *mem_Malloc(uint16_t size);
void *mem_Calloc(uint16_t size);
void *mem_Realloc(void *ptr);
int mem_Free(void *ptr);

```

`mem_init`是用于CCMRAM内存池初始化的函数，应在调用后续所有内存操作函数之前调用一次！！！

## 内部私有函数和私有变量

```c

__attribute__((section(".ccmram"))) static uint8_t gCCMRAM[_MAX_CCMRAM_HEAP_SIZE * 1024];
static uint32_t idx = 0;

inline static void _mem_merge(void);
__attribute__((weak)) void Error_Mem_Get(void);

```

- `gCCMRAM[_MAX_CCMRAM_HEAP_SIZE * 1024]`作为内存池供后续函数调用
- `idx`作为全局静态索引id分配给内存块使用，只增不减，同时兼顾内存池初始化判断
- `_mem_merge`合并未使用内存，在每次调用`mem_Free`成功后调用
- `Error_Mem_Get`在每次内存分配失败后调用，默认为不作任何操作，待完善
