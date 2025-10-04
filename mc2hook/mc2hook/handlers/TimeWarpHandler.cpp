#include "TimeWarpHandler.h"

void TimeWarpHandler::Install()
{
    float timeWarp = HookConfig::GetFloat("General", "TimeWarp", 1.0f);
    mem::write(0x613FA9 + 6, static_cast<float>(timeWarp));
    mem::write(0x4CFBD3 + 6, static_cast<float>(timeWarp));
}
