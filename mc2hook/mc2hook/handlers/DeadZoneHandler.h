#pragma once
#include <mc2hook\mc2hook.h>

class DeadZoneHandler
{
public:
    static float ioAddDeadZone(float originalValue, float deadZone);
    static void Install();
};
