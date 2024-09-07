#include "log.h"
#include <stdarg.h>
#include <stdint.h>

#include "SEGGER/Config/SEGGER_RTT_Conf.h"

void 
LogInit(void) { SEGGER_RTT_Init(); }

void 
LogColorSet(const int channel, const char *color) {
    if (!(channel == dbgout || channel == dbgwarn || channel == dbgerr)) {
        return;
    }
    SEGGER_RTT_WriteString(channel, color);
}

__attribute__((format(printf, 2, 3))) int 
printLog(const int channel, const char *fmt, ...) {
    if (!(channel == dbgout || channel == dbgwarn || channel == dbgerr)) {
        return -1;
    }

    // 原颜色切换代码，已移动至LogColorSet函数中实现
    // switch (channel) {
    //     case dbgout:
    //         SEGGER_RTT_WriteString(dbgout, RTT_CTRL_TEXT_BRIGHT_WHITE);
    //     case dbgwarn:
    //         SEGGER_RTT_WriteString(dbgwarn, RTT_CTRL_TEXT_BRIGHT_YELLOW);
    //     case dbgerr:
    //         SEGGER_RTT_WriteString(dbgerr, RTT_CTRL_TEXT_BRIGHT_RED);
    // }

    va_list args;
    va_start(args, fmt);
    int n = SEGGER_RTT_vprintf(channel, fmt, &args); 
    va_end(args);

    // SEGGER_RTT_WriteString (channel, RTT_CTRL_RESET);

    return n;
}
