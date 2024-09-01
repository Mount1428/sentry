#ifndef _LIB_INIT_H
#define _LIB_INIT_H

#include "log/log.h"
#include "mm/mem.h"
#include "time/time.h"

void
__user_lib_init(void) {
    _time_init(168);
    mem_init();
    LogInit();
}

#endif // !_LIB_INIT_H
