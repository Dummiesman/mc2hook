#include "vehTransmisisonHandler.h"

void vehTransmissionHandler::Install()
{
    // In Manual mode, set min gear to 0 instead of 2
    // allows MT to enter reverse
    // though, replays break when you do this
    mem::write(0x567DBA + 2, static_cast<uint8_t>(0));
}
