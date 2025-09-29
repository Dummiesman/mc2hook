#pragma once
#include <mc2hook\mc2hook.h>

class FPSCounterHandler
{
private:
    static hook::TypeProxy<float> s_FrameTime;
public:
    void DrawFontHook(int x, int y, void* a3, void* a4);
    static void Install();
};
