#pragma once
#include <string.h>
#include <age\memory\memory.h>

static wchar_t* WStringDuplicate(const wchar_t* str)
{
    if (!str) return nullptr;
    int len = wcslen(str) + 1;
    wchar_t* dup = (wchar_t*)age_allocate(len * sizeof(wchar_t));
    memcpy(dup, str, len * sizeof(wchar_t));
    return dup;
}