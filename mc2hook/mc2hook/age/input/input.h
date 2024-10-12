#pragma once
#include "keyboard.h"

class ioInput
{
public:
    static hook::Type<bool> bUseKeyboard;
    static hook::Type<bool> bUseJoystick;
};