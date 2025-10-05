#pragma once
#include <mc2hook\mc2hook.h>
#include <age/vector/vector3.h>

class LevelExtentsHandler
{
public:
    void InitRoomsHook(const Vector3& a2, const Vector3& a3, const float& a4);
    static void Install();
};
