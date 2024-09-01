#ifndef _LOG_H
#define _LOG_H

#include "SEGGER/RTT/SEGGER_RTT.h"

#define dbgout  0   // 标准输出
#define dbgwarn 1   // 警告输出（一般不用）
#define dbgerr  2   // 错误输出

/// @brief 日志系统初始化
/// @param  
void LogInit(void);

/// @brief 日志输出通道颜色设置
/// @param channel 
/// @param color
void LogColorSet(const int channel, const char *color);

/// @brief 日志输出函数
/// @param channel 输出通道
/// @param fmt 格式字符串
/// @param ... 参数列表
/// @return int 打印的字符数（打印失败返回-1）
int printLog(const int channel, const char *fmt, ...);

/*------下面是日志输出的接口--------*/
/*------仅支持dbgout标准输出-------*/
/*---后续转换为printLog进行输出（类比Linux终端）---*/

/**
 * @brief 日志功能原型,供下面的LOGI,LOGW,LOGE等使用
 *
 */
#define LOG_PROTO(type, color, format, ...)                                    \
  SEGGER_RTT_printf(dbgout, "  %s%s" format "\r\n%s", color, type,       \
                    ##__VA_ARGS__, RTT_CTRL_RESET)

/* 清屏 */
#define LOG_CLEAR() SEGGER_RTT_WriteString(dbgout, "  " RTT_CTRL_CLEAR)

/* 无颜色日志输出 */
#define LOG(format, ...) LOG_PROTO("", "", format, ##__VA_ARGS__)

/* 有颜色格式日志输出,建议使用这些宏来输出日志 */
// information level
#define LOGINFO(format, ...)                                                   \
  LOG_PROTO("I", RTT_CTRL_TEXT_BRIGHT_GREEN, format, ##__VA_ARGS__)
// warning level
#define LOGWARNING(format, ...)                                                \
  LOG_PROTO("W", RTT_CTRL_TEXT_BRIGHT_YELLOW, format, ##__VA_ARGS__)
// error level
#define LOGERROR(format, ...)                                                  \
  LOG_PROTO("E", RTT_CTRL_TEXT_BRIGHT_RED, format, ##__VA_ARGS__)

#endif // !_LOG_H

