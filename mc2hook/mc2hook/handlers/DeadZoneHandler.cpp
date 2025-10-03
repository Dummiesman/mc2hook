#include "DeadZoneHandler.h"
#include <age/input/input.h>

float deadZoneMultiplier;

float DeadZoneHandler::ioAddDeadZone(float originalValue, float deadZone)
{
    originalValue *= deadZoneMultiplier;
    return ioInput::ioAddDeadZone(originalValue, deadZone);
}

void DeadZoneHandler::Install()
{
    deadZoneMultiplier = HookConfig::GetFloat("Input", "DeadZoneMultiplier", 1.0f);

    InstallCallback("Deadzone Handler", "ioAddDeadZone",
        &ioAddDeadZone, {
            cb::call(0x426603),
            cb::call(0x46752D),
            cb::call(0x561D77),
            cb::call(0x561D95),
            cb::call(0x5689E0),
            cb::call(0x60492A),
        });
}
