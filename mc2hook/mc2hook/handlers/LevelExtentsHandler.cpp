#include "LevelExtentsHandler.h"
#include <age/vector/vector3.h>

void LevelExtentsHandler::InitRoomsHook(const Vector3& a2, const Vector3& a3, const float& a4)
{
    float worldMinX = HookConfig::GetFloat("Physics", "WorldExtentMinX", -2000.0f);
    float worldMaxX = HookConfig::GetFloat("Physics", "WorldExtentMaxX", 2000.0f);

    float worldMinZ = HookConfig::GetFloat("Physics", "WorldExtentMinZ", -2000.0f);
    float worldMaxZ = HookConfig::GetFloat("Physics", "WorldExtentMaxZ", 2000.0f);

    Vector3 worldMin = Vector3(worldMinX, a2.Y, worldMinZ);
    Vector3 worldMax = Vector3(worldMaxX, a3.Y, worldMaxZ);

    hook::Thunk<0x587850>::Call<void>(this, &worldMin, &worldMax, &a4); // Call original
}

void LevelExtentsHandler::Install()
{
    InstallCallback("Level Extents Handler", "Level Extents SetRoomConfig hook.",
        &InitRoomsHook, {
            cb::call(0x40B156),
            cb::call(0x589239)
        }
    );
}
