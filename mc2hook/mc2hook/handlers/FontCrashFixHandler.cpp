#include "FontCrashFixHandler.h"

void FontCrashFixHandler::Install()
{
    // nop out font cleanup
    // fixes crash on exit which is usually invisible (but is now visible due to our exception handler)
    mem::nop(0x53A870, 5);
}
