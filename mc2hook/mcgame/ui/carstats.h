#pragma once
#include <mc2hook\mc2hook.h>

struct mcFeCarStats_CarStat
{
    LPCSTR basename;
    int index;
    float carSpeed;
    float carHandling;
    float carAcceleration;
    int carNitros;
    int equipmentIndex;
};

struct mcFeCarStats_SponsorInfo
{
    LPCSTR strIconSource;
    LPCSTR basename;
    LPWSTR strEquip;
    /*gfxTexture* */ void* texIcon;
};
