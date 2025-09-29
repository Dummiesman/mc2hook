#include "FPSCounterHandler.h"
#include <age/gfx/font.h>
#include <age/gfx/pipeline.h>

declfield(FPSCounterHandler::s_FrameTime) = 0x858348;

char fpsString[64];
float fps;

void FPSCounterHandler::DrawFontHook(int x, int y, void* a3, void* a4)
{
    fps = 1000.0f / s_FrameTime; // Originally in the code it divides 1010.0f?
    sprintf(fpsString, "%.0f", fps);
    gfxFont::DrawFont(gfxPipeline::m_iWidth - (8 * strlen(fpsString) + 40), y, fpsString, a4);
}

void FPSCounterHandler::Install()
{
    bool enableFPSCounter = HookConfig::GetBool("Graphics", "EnableFPSCounter", false);

    if (enableFPSCounter)
    {
        mem::write(0x858330, static_cast<int>(1)); // Forces -frametime on
        InstallCallback("FPS Counter", "FPS Counter",
            &DrawFontHook, {
                cb::call(0x5ED1A1)
            });
    }
}
