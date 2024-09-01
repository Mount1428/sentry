# libs

> 提供全局调用函数（允许在application层依旧调用）
> 
> log部分移植自原bsp/log
>

## 代码结构

```dir
    .
    |
    |--- algorithm
    |           |--- crc
    |           |--- pid
    |           |--- user_libs
    |--- log
    |--- mm
```

## 注意事项

`__init__.h`中`__user_lib_init()`函数应在HAL库初始化完成后立即调用
