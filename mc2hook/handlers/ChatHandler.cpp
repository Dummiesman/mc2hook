#include "ChatHandler.h"

void ChatHandler::Install()
{
    // nop out built in "keylogger"
    // this logged each keypress to the log/console while chatting in multiplayer
    mem::nop(0x44F660, 5);
}
