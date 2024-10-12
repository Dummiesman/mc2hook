#include "RemoveForcedArgsHandler.h"

void RemoveForcedArgsHandler::Install()
{
    // Remove forced arguments
    mem::nop(0x40119B, 5);
}
