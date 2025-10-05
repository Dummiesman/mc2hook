#pragma once
#include <mc2hook\mc2hook.h>

class gfxViewport
{
public:
    static void Perspective(float const& fov, float const& aspect, float const& near_clip, float const& far_clip);
};
