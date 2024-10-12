#pragma once
#include <mc2hook\mc2hook.h>

class zipEnumHandler
{
public:
    void EnumFiles(LPCSTR path, void(__cdecl* cb)(const char*, bool, void*), void* userdata);
    static void Install();
};