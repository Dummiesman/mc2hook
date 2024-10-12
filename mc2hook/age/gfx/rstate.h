#pragma once
#include <mc2hook\mc2hook.h>
#include <age/vector/matrix44.h>
#include <age/vector/matrix34.h>

class gfxRenderState
{
private:
    static hook::TypeProxy<Matrix44> sm_Camera;
public:
    static void SetCamera(Matrix44 const& mtx);
    static void SetCamera(Matrix34 const& mtx);
public:
    static Matrix44 GetCameraMatrix();
};