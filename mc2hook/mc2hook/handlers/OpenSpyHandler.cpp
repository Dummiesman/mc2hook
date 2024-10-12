#include "OpenSpyHandler.h"

void OpenSpyHandler::Install()
{
    // replace 'gamespy.com' portion of the following URLs with 'openspy.net'
    mem::write_string(0x64F3D4, "openspy.net"); // %s.ms%d.gamespy.com
    mem::write_string(0x64F45C, "openspy.net"); // natneg2.gamespy.com
    mem::write_string(0x64F470, "openspy.net"); // natneg1.gamespy.com
    mem::write_string(0x64F4AA, "openspy.net"); // %s.master.gamespy.com
}   