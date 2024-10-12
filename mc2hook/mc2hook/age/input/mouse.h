#pragma once
#include <mc2hook\mc2hook.h>
#include <dinput.h>

class ioMouse
{
private:
    static hook::Type<int> m_X;
    static hook::Type<int> m_Y;
    static hook::Type<int> m_dX;
    static hook::Type<int> m_dY;
    static hook::Type<int> m_dZ;
    static hook::Type<int> m_LastButtons;
    static hook::Type<int> m_Buttons;
    static hook::Type<int> m_WindowButtons;
    static hook::Type<bool> m_UseWindow;
    static hook::Type<IDirectInputDeviceA*> mouseDev;
public:
    static int GetX() {
        return m_X.get();
    }

    static int GetY() {
        return m_Y.get();
    }

    static int GetXDelta() {
        return m_dX.get();
    }

    static int GetYDelta() {
        return m_dY.get();
    }

    static int GetScrollDelta() {
        return m_dZ.get();
    }

    static bool GetButtonUp(int button) {
        int flag = 1 << button;

        bool last = (m_LastButtons.get() & flag) != 0;
        bool current = (m_Buttons.get() & flag) != 0;
        return last && !current;
    }

    static bool GetButtonDown(int button) {
        int flag = 1 << button;

        bool last = (m_LastButtons.get() & flag) != 0;
        bool current = (m_Buttons.get() & flag) != 0;
        return current && !last;
    }

    static bool GetButton(int button) {
        int flag = 1 << button;

        bool current = (m_Buttons.get() & flag) != 0;
        return current;
    }

    static void ClearStates() {
        m_dX.set(0);
        m_dY.set(0);
        m_dZ.set(0);
        m_LastButtons.set(0);
        m_Buttons.set(0);
        m_WindowButtons.set(0);
    }
};