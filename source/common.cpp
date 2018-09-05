#include "common.hpp"

#include <switch/kernel/svc.h>

Result SleepNano(u64 nano)
{
    return svcSleepThread(nano);
}
