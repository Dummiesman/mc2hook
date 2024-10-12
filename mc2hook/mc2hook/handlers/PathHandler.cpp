#include "PathHandler.h"
#include <age\data\asset.h>
#include <age\data\args.h>

bool PathHandler::ArgGetHook(LPCSTR arg, int index, const char** out)
{
    // Originally a call to datArgParser::Get("path", 0, ...);
    if (!datArgParser::Get(arg, index, out))
    {
        if (strlen(datAssetManager::GetPath()) > 0)
        {
            *out = datAssetManager::GetPath();
        }
        else
        {
            *out = ".";
        }
    }
    return true;
}

void PathHandler::Install()
{
    InstallCallback("Path Handler", "Hook multiple getters of the -path arg and redirect them to datAssetManager::GetPath() when not present.",
        &ArgGetHook, {
            cb::call(0x40ABEA),
            cb::call(0x40B63A),
            cb::call(0x457025)
        }
    );
}
