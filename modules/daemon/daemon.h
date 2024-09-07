#ifndef _DAEMON_H
#define _DAEMON_H

#include <stdint.h>

#define MAX_DAEMON_COUNT 32

// Daemon模块指向实例的状态
typedef enum
{
    DAEMON_NONE = 0b0000,    // 无错误
    DAEMON_WARNING = 0b0001, // 有错误发生但不会造成机器失控
    DAEMON_ERROR = 0b0010,   // 有错误发生且存在机器失控可能
} Daemon_Status_e;

typedef struct _daemon
{
    uint16_t reload_count;          // 重载值,单位 ms
    Daemon_Status_e daemon_status;  // Daemon模块指向实例的状态
    Daemon_Status_e offline_status; // 离线时模块指向实例的状态

    void (*offline_callback)(struct _daemon *); // 离线错误回调函数
    void *id;                                   // 父实例指针，用于触发离线错误回调函数时快速定位离线模块

    int16_t temp_count;
} DaemonInstance;

typedef void (*offline_callback)(DaemonInstance *);

typedef struct
{
    uint16_t reload_count;         // 重载值
    uint16_t init_count;           // 初始化等待值
    Daemon_Status_e daemon_status; // Daemon模块指向实例的状态

    offline_callback callback; // 离线错误回调函数
    void *owner_id;                          // 父实例指针，用于触发离线错误回调函数时快速定位离线模块
} Daemon_Init_Config_s;

/// @brief Daemon模块初始化函数
/// @param
void DaemonInit(void);

/// @brief Daemon实例注册函数
/// @param config 初始化配置
/// @return DaemonInstance* 返回实例指针
DaemonInstance *DaemonRegister(Daemon_Init_Config_s *config);

/// @brief 喂狗函数
/// @param ins
void DaemonReload(DaemonInstance *ins);

/// @brief Daemon模块是否离线
/// @param ins
/// @return
uint8_t DaemonIsOnline(DaemonInstance *ins);

/// @brief Daemon任务函数
/// @param
void DaemonTask(void);

#endif // !_DAEMON_H
