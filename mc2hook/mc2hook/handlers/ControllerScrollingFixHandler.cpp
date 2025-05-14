#include "ControllerScrollingFixHandler.h"

void ControllerScrollingFixHandler::Install()
{
    bool enableMenuScrollFix = HookConfig::GetBool("Experimental", "MenuScrollFix", true);
    if (enableMenuScrollFix)
    {
        // Fixes controller scrolling bug in the menu
        mem::write(0x603EBA, 0x90E9);
    }
}