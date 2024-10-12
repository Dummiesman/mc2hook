#pragma once
#include <mc2hook\mc2hook.h>

// A few functions in the game *require* a -path argument
// This handler adds some extra code that uses datAssetManager path as a fallback
// Then if that's also not present, it will return "."
// This fixes failure to load some files (e.g. cpvs) if the -path argument is missing

class PathHandler
{
public:
    static bool ArgGetHook(LPCSTR arg, int index, const char** out);
    static void Install();
};