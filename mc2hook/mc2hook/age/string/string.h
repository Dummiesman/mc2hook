#pragma once
#include <string.h>
#include <age\memory\memory.h>

static char* StringDuplicate(const char* str)
{
    if (!str) return nullptr;
    int size = strlen(str) + 1;
    char* dup = (char*)age_allocate(size);
    memcpy(dup, str, size);
    return dup;
}