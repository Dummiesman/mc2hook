#pragma once
#include <mc2hook\mc2hook.h>
#include <age/vector/matrix34.h>

class FreeCamHandler
{
public:
    static void SetCameraHook(Matrix34& const OrigMtx);
    static void SetCameraOtherHook(Matrix34& const OrigMtx);
    static void SetWorldHook(Matrix34& const OrigMtx);
    void AddFrameCameraHook(Matrix34& const a2, bool& const a3);
    void sub_518FA0_Hook(Vector3& const a2, void* a3, void* a4);
    void PerspectiveHook(float fov, float& const aspect, float& const near_clip, float& const far_clip);
    void ToggleCinematicCamHook();
    float GetFOVLerpRate(float a, float b, float t);

    static void Install();
};
