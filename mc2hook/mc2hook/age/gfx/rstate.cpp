#include "rstate.h"

declfield(gfxState::sm_Camera) = 0x858548;

void gfxState::SetCamera(Matrix44 const& mtx) 
{
    hook::StaticThunk<0x5EE8D0>::Call<void>(&mtx); 
}

void gfxState::SetCamera(Matrix34 const& mtx) 
{
    hook::StaticThunk<0x5EE900>::Call<void>(&mtx); 
}

Matrix44 gfxState::GetCameraMatrix() 
{
    return gfxState::sm_Camera;
}

void gfxState::SetCull(gfxCullMode mode)
{
    hook::StaticThunk<0x5EE1A0>::Call<void>(static_cast<int>(mode));
}

void gfxState::SetZWriteEnable(bool enable)
{
    hook::StaticThunk<0x5EE4A0>::Call<void>(enable);
}
