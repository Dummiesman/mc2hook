#pragma once
#include <mc2hook\mc2hook.h>

class CarShadowHandler
{
public:
    static void SetColor_Hook(float a1);
    static void Install();
};
