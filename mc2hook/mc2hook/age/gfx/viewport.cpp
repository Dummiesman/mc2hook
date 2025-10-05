#include "viewport.h"

void gfxViewport::Perspective(float const& fov, float const& aspect, float const& near_clip, float const& far_clip)
{
    hook::StaticThunk<0x5FA880>::Call<void>(&fov, &aspect, &near_clip, &far_clip);
}
