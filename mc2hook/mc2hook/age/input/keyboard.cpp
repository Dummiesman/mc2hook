#include "keyboard.h"

declfield(ioKeyboard::m_Active)(0x85F888);
declfield(ioKeyboard::m_Keys)(0x85F898);

#define KEYINDEX(active, index) ((256 * active) + index)

bool ioKeyboard::GetKey(uint8_t keycode)
{
    return m_Keys.ptr()[KEYINDEX(m_Active.get(), keycode)];
}

bool ioKeyboard::GetKeyDown(uint8_t keycode)
{
    return (m_Keys.ptr()[KEYINDEX(0, keycode)] ^ m_Keys.ptr()[KEYINDEX(1, keycode)])
        & m_Keys.ptr()[KEYINDEX(GetActiveFlag(), keycode)];
}

bool ioKeyboard::GetKeyUp(uint8_t keycode)
{
    return (m_Keys.ptr()[KEYINDEX(0, keycode)] ^ m_Keys.ptr()[KEYINDEX(1, keycode)])
        & m_Keys.ptr()[KEYINDEX(!GetActiveFlag(), keycode)];
}
