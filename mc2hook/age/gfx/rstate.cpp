#include "rstate.h"

declfield(gfxRenderState::sm_Camera) = 0x858548;

inline void gfxRenderState::SetCamera(Matrix44 const& mtx) {
    hook::StaticThunk<0x5EE8D0>::Call<void>(&mtx); 
}

inline void gfxRenderState::SetCamera(Matrix34 const& mtx) {
    hook::StaticThunk<0x5EE900>::Call<void>(&mtx); 
}

inline Matrix44 gfxRenderState::GetCameraMatrix() {
    return gfxRenderState::sm_Camera;
}
