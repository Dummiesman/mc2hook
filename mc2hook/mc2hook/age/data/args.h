#pragma once
#include <mc2hook/mc2hook.h>

class datArgParser
{
public:
    static bool Get(LPCSTR arg)
    {
        return hook::StaticThunk<0x612E10>::Call<bool>(arg);
    }

    static int GetNum(LPCSTR arg)
    {
        return hook::StaticThunk<0x612EE0>::Call<int>(arg);
    }

    static bool Get(LPCSTR arg, int argIndex, char const** outStr)
    {
        return hook::StaticThunk<0x612EB0>::Call<bool>(arg, argIndex, outStr);
    }
};