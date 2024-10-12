#pragma once
#include "group.h"

class mcFeMainMenu : public mcUiGroup
{
private:
    uint8_t buf[0x64];
    int32_t colors1[32];
    int32_t colors2[32];
public:
    void ResetColorSelections()
    {
        for (int i = 0; i < 32; i++)
        {
            colors1[i] = 0;
            colors2[i] = 0;
        }
    }
};
ASSERT_SIZEOF(mcFeMainMenu, 0x1C4);

class mcFeMainMenu_new : public mcFeMainMenu
{
private:
    int32_t colors1[512];
    int32_t colors2[512];
public:
    void ResetColorSelections()
    {
        for (int i = 0; i < 512; i++)
        {
            colors1[i] = 0;
            colors2[i] = 0;
        }
    }
};