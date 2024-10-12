#pragma once
#include <mc2hook\mc2hook.h>

// Very rudimentary smooth steering handler for digital inputs
class SteeringSmootherHandler
{
public:
    void Update();
    static void Install();
};