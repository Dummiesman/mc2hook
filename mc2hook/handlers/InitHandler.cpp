#include "InitHandler.h"
#include <mc2hook\mc2hook.h>
#include <handlers\handlers.h>

void InitHandler::PostEngineInitHook()
{
    InstallHandlersPostEngineInit();
}

void InitHandler::PreEngineInitHook()
{
    InstallHandlersPreEngineInit();
}

void InitHandler::Install()
{
    // Hooks function that forces game arguments to "mc.exe -path=. -archive=assets_p.dat"
    InstallCallback("GameInitHook", "Hook game init to allow installing handlers at an appropriate time",
        &PreEngineInitHook, {
            cb::call(0x40119B),
        }
    );

    // Hooks a nullsub
    InstallCallback("GameInitHook", "Hook game init to allow installing handlers at an appropriate time",
        &PostEngineInitHook, {
            cb::call(0x401384),
        }
    );
}
