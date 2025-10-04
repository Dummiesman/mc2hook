#pragma once
#include <mc2hook\mc2hook.h>

class ioKeyboard
{
private:
    static hook::Type<int> m_Active;
    static hook::Type<uint8_t[512]> m_Keys;

    static bool GetActiveFlag()
    {
        return m_Active != 0;
    }

public:
    static bool GetKey(uint8_t keycode);
    static bool GetKeyDown(uint8_t keycode);
    static bool GetKeyUp(uint8_t keycode);
};