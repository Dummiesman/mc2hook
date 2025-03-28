#include "ControllerScrollingFixHandler.h"

void ControllerScrollingFixHandler::Install()
{
    // Fixes controller scrolling bug in the menu
    mem::write(0x603EBA, 0x90E9);
}