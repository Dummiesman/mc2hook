#pragma once
#include <mc2hook\mc2hook.h>

// The primary function of this class is to allow automatic loading of all .dat in the current directory
class zipInitHandler
{
public:
    static void zipAutoInit();
    static void zipMultiAutoInit(LPCSTR filter);
    static void zipInit();
    static void Install();
};