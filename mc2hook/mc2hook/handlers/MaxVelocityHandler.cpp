#include "MaxVelocityHandler.h"

void MaxVelocityHandler::Install()
{
    float maxVelocity = HookConfig::GetFloat("Physics", "MaxVelocity", 111.75 * 2.237); // In mph format
    mem::write(0x569195 + 3, static_cast<float>(maxVelocity / 2.237)); // mph to ms
}
