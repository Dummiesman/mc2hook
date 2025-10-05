#include "FreecamHandler.h"
#include <age/vector/matrix34.h>
#include <age/vector/matrix44.h>
#include <age/gfx/rstate.h>
#include <age/gfx/viewport.h>
#include <age/core/output.h>
#include <age/input/keyboard.h>
#include <age/input/mouse.h>
#include <age/data/timemgr.h>
#include <dinput.h>

// TODO:
// Attach rain particles to the camera and not the car
// Create a FreeCamHandler::Update instead of 2 SetCamera functions
// Make a standard Lerp function
// Add boost mode with Shift
// Add overall smoothing, for movement and rotation
// Disable HUD
// Add camera roll, and a way to reset back to 0
// Expose speed, sensitivity and smoothing variables to .ini
// Clean up and split things up into neater functions

// Disable game input and use WASD(QE) instead
// Add a way to increase/decrease fog distance, for nice distant shots

// Init variables
bool OverrideCameraMtx = false;
bool FreecamToggle = false;
Matrix34 CustomCameraMtx34 = Matrix34::I;
Matrix34 CustomSkyboxCameraMtx34 = Matrix34::I;

Vector3 Dir;
Vector3 E;

float translateXZSpeed, translateYSpeed;
float translateMultiplier = 1.0f; // Initial translate speed multiplier
float translateMultiplierStep, minTranslateMultiplier, maxTranslateMultiplier;

float rotYSensitivity, rotXSensitivity;

float customFOV = 30.0f; // Initial custom FOV
float minFOV, maxFOV, fovStep;

float fovRotSensitivity = 1.0f;

static int currentGameState = 0;

float cameraNearClip, cameraFarClip;

// Init controls
int activateFreecam, freecamLeft, freecamRight, freecamForward, freecamBackward,
freecamUp, freecamDown, freecamIncreaseFOV, freecamDecreaseFOV, activateReplayCam;

void FreeCamHandler::SetCameraHook(Matrix34& const OrigMtx)
{
    if (ioKeyboard::GetKeyDown(activateFreecam) && !FreecamToggle)//(GetAsyncKeyState(0xA2) & 1) // Activates free cam on LCTRL, https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    {
        FreecamToggle = true;
        Printf("Free camera ON\n");

        // Toggle override
        OverrideCameraMtx = !OverrideCameraMtx;

        // Executes once on buttom press, could be its own separate Init function
        if (OverrideCameraMtx)
        {
            // Place the custom camera matrix at the current one
            Matrix44 CurrentCameraMtx44 = gfxState::GetCameraMatrix();

            // This could be done using a separate utility function converting Matrix44 to Matrix34
            // CustomCameraMtx34 = gfxRenderState::GetCameraMatrix().ToMatrix34();

            Vector4 m44a = CurrentCameraMtx44.GetRow(0);
            Vector4 m44b = CurrentCameraMtx44.GetRow(1);
            Vector4 m44c = CurrentCameraMtx44.GetRow(2);
            Vector4 m44d = CurrentCameraMtx44.GetRow(3);

            CustomCameraMtx34.SetRow(0, Vector3(m44a.X, m44a.Y, m44a.Z));
            CustomCameraMtx34.SetRow(1, Vector3(m44b.X, m44b.Y, m44b.Z));
            CustomCameraMtx34.SetRow(2, Vector3(m44c.X, m44c.Y, m44c.Z));
            CustomCameraMtx34.SetRow(3, Vector3(m44d.X, m44d.Y, m44d.Z));

            //mem::nop(0x49E234, 3); //this doesn't disable the hud?
        }
    }

    if (ioKeyboard::GetKeyUp(activateFreecam) && FreecamToggle)
    {
        FreecamToggle = false;
    }

    // Mouse delta values used for camera rotation
    int DX = ioMouse::GetXDelta();
    int DY = ioMouse::GetYDelta();
    int DZ = ioMouse::GetScrollDelta();

    if (DX != 0 || DY != 0) // Update only when mouse moves
    {
        Matrix34 ry;
        ry.MakeRotateY(-rotYSensitivity * DX * fovRotSensitivity); // Rotates left/right in world space
        CustomCameraMtx34.Dot3x3(ry);

        Matrix34 rx;
        rx.MakeRotate(CustomCameraMtx34.GetRow(0), -rotXSensitivity * DY * fovRotSensitivity); // Rotates up/down in local space
        CustomCameraMtx34.Dot3x3(rx);
    }

    // Camera speed adjustment with mouse scroll wheel
    if (DZ > 0 && translateMultiplier < maxTranslateMultiplier)
    {
        translateMultiplier += translateMultiplierStep;
    }

    if (DZ < 0 && translateMultiplier > minTranslateMultiplier)
    {
        translateMultiplier -= translateMultiplierStep;
    }

    if (DZ < 0 && translateMultiplier < minTranslateMultiplier)
    {
        translateMultiplier = minTranslateMultiplier; // Overshoot safeguard
    }

    // Move camera forth, left, back, right - relative to camera direction
    if (ioKeyboard::GetKey(freecamForward))
    {
        // Simplify these

        Dir.X = CustomCameraMtx34.m20;
        Dir.Y = CustomCameraMtx34.m21;
        Dir.Z = CustomCameraMtx34.m22;

        E.X = Dir.X * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Y = Dir.Y * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Z = Dir.Z * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;

        CustomCameraMtx34.m30 -= E.X;
        CustomCameraMtx34.m31 -= E.Y;
        CustomCameraMtx34.m32 -= E.Z;
    }
    if (ioKeyboard::GetKey(freecamLeft))
    {
        Dir.X = CustomCameraMtx34.m00;
        Dir.Y = CustomCameraMtx34.m01;
        Dir.Z = CustomCameraMtx34.m02;

        E.X = Dir.X * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Y = Dir.Y * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Z = Dir.Z * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;

        CustomCameraMtx34.m30 -= E.X;
        CustomCameraMtx34.m31 -= E.Y;
        CustomCameraMtx34.m32 -= E.Z;
    }
    if (ioKeyboard::GetKey(freecamBackward))
    {
        Dir.X = CustomCameraMtx34.m20;
        Dir.Y = CustomCameraMtx34.m21;
        Dir.Z = CustomCameraMtx34.m22;

        E.X = Dir.X * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Y = Dir.Y * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Z = Dir.Z * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;

        CustomCameraMtx34.m30 += E.X;
        CustomCameraMtx34.m31 += E.Y;
        CustomCameraMtx34.m32 += E.Z;
    }
    if (ioKeyboard::GetKey(freecamRight))
    {
        Dir.X = CustomCameraMtx34.m00;
        Dir.Y = CustomCameraMtx34.m01;
        Dir.Z = CustomCameraMtx34.m02;

        E.X = Dir.X * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Y = Dir.Y * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;
        E.Z = Dir.Z * datTimeManager::GetSeconds() * translateXZSpeed * translateMultiplier;

        CustomCameraMtx34.m30 += E.X;
        CustomCameraMtx34.m31 += E.Y;
        CustomCameraMtx34.m32 += E.Z;
    }

    // Move camera down and up
    if (ioKeyboard::GetKey(freecamDown))
    {
        CustomCameraMtx34.m31 -= datTimeManager::GetSeconds() * translateYSpeed * translateMultiplier;
    }
    if (ioKeyboard::GetKey(freecamUp))
    {
        CustomCameraMtx34.m31 += datTimeManager::GetSeconds() * translateYSpeed * translateMultiplier;
    }

    if (OverrideCameraMtx)
    {
        gfxState::SetCamera(CustomCameraMtx34);
    }
    else
    {
        gfxState::SetCamera(OrigMtx); // Call original
    }
}
void FreeCamHandler::SetCameraOtherHook(Matrix34& const OrigMtx) // Only set the matrix, no other calculations
{
    if (OverrideCameraMtx)
    {
        gfxState::SetCamera(CustomCameraMtx34);
    }
    else
    {
        gfxState::SetCamera(OrigMtx); // Call original
    }
}
void FreeCamHandler::SetWorldHook(Matrix34& const OrigMtx)
{
    if (OverrideCameraMtx)
    {
        CustomSkyboxCameraMtx34 = OrigMtx;
        CustomSkyboxCameraMtx34.SetRow(3, CustomCameraMtx34.GetRow(3));
        gfxState::SetWorld(CustomSkyboxCameraMtx34); // Call using custom matrix
    }
    else
    {
        gfxState::SetWorld(OrigMtx); // Call original
    }
}
void FreeCamHandler::AddFrameCameraHook(Matrix34& const a2, bool& const a3)
{
    if (OverrideCameraMtx)
    {
        hook::Thunk<0x5D6050>::Call<void>(this, &CustomCameraMtx34, false); // Call using custom camera matrix
    }
    else
    {
        hook::Thunk<0x5D6050>::Call<void>(this, &a2, &a3); // Call original
    }
}
void FreeCamHandler::sub_518FA0_Hook(Vector3& const a2, void* a3, void* a4)
{
    if (OverrideCameraMtx)
    {
        a2.X = CustomCameraMtx34.m30;
        a2.Z = CustomCameraMtx34.m32;
        hook::Thunk<0x518FA0>::Call<void>(this, &a2, a3, a4); // Call using custom camera position
    }
    else
    {
        hook::Thunk<0x518FA0>::Call<void>(this, &a2, a3, a4); // Call original
    }
}
void FreeCamHandler::PerspectiveHook(float fov, float& const aspect, float& const near_clip, float& const far_clip)
{
    if (OverrideCameraMtx)
    {
        // Custom FOV logic
        if (ioKeyboard::GetKey(freecamIncreaseFOV))
        {
            if (customFOV < maxFOV)
            {
                customFOV += fovStep * datTimeManager::GetSeconds();// Increase FOV (FPS independent)
                fovRotSensitivity = GetFOVLerpRate(0.25f, 2.0f, customFOV / maxFOV);
            }
        }

        if (ioKeyboard::GetKey(freecamDecreaseFOV))
        {
            if (ceil(customFOV) > minFOV)
            {
                customFOV -= fovStep * datTimeManager::GetSeconds();// Decrease FOV (FPS independent)
                fovRotSensitivity = GetFOVLerpRate(0.25f, 2.0f, customFOV / maxFOV);
            }
        }

        //gfxViewport::Perspective(10.0f, &aspect, &near_clip, &far_clip);
        hook::Thunk<0x5FA880>::Call<void>(this, customFOV, &aspect, cameraNearClip, cameraFarClip);
    }
    else
    {
        //gfxViewport::Perspective(&fov, &aspect, &near_clip, &far_clip); // Call original
        hook::Thunk<0x5FA880>::Call<void>(this, fov, &aspect, cameraNearClip, cameraFarClip); // Call original
        customFOV = fov; // Grab current FOV
    }
}
void FreeCamHandler::ToggleCinematicCamHook()
{
    hook::Thunk<0x4045E0>::Call<void>(this); // Call original function (some mcGameState Update it seems)

    if (ioKeyboard::GetKeyDown(activateReplayCam)) //(GetAsyncKeyState(0x69) & 1) // NUMPAD9
    {
        Printf("Cinematic camera toggled\n");
        currentGameState = *getPtr<int>(this, 0x4);

        if (currentGameState == 2) // If current state is GAMEPLAY
        {
            *getPtr<int>(this, 0x4) = 3; // Switch to REPLAY state
            // Look into the original EnterStateReplay function and see how it handles car LODs
        }
        else if (currentGameState == 3) // If current state is REPLAY
        {
            *getPtr<int>(this, 0x4) = 2; // Switch to GAMEPLAY state
            // Need to refresh camera here, do it manually in game for now
        }
    }
}
float FreeCamHandler::GetFOVLerpRate(float a, float b, float t) // Regular lerp function, replace it later
{
    return a + (b - a) * t;
}

void FreeCamHandler::Install()
{
    bool enableFreecam = HookConfig::GetBool("Freecam", "EnableFreecam", true);

    if (enableFreecam)
    {
        // Read key mappings from .ini file
        activateFreecam = HookConfig::GetInt("Freecam Controls", "ActivateFreecam", DIK_LCONTROL);
        freecamLeft = HookConfig::GetInt("Freecam Controls", "FreecamLeft", DIK_J);
        freecamRight = HookConfig::GetInt("Freecam Controls", "FreecamRight", DIK_L);
        freecamForward = HookConfig::GetInt("Freecam Controls", "FreecamForward", DIK_I);
        freecamBackward = HookConfig::GetInt("Freecam Controls", "FreecamBackward", DIK_K);
        freecamUp = HookConfig::GetInt("Freecam Controls", "FreecamUp", DIK_O);
        freecamDown = HookConfig::GetInt("Freecam Controls", "FreecamDown", DIK_U);
        freecamIncreaseFOV = HookConfig::GetInt("Freecam Controls", "FreecamIncreaseFOV", DIK_P);
        freecamDecreaseFOV = HookConfig::GetInt("Freecam Controls", "FreecamDecreaseFOV", DIK_SEMICOLON);
        activateReplayCam = HookConfig::GetInt("Freecam Controls", "ActivateReplayCam", DIK_NUMPAD9);

        // Read camera clip values
        cameraNearClip = HookConfig::GetFloat("Graphics", "CameraNearClip", 0.25f);
        cameraFarClip = HookConfig::GetFloat("Graphics", "CameraFarClip", 2000.0f);

        // Read freecam variables
        translateXZSpeed = HookConfig::GetFloat("Freecam", "TranslateXZSpeed", 20.0f); // Speed on the XZ plane
        translateYSpeed = HookConfig::GetFloat("Freecam", "TranslateYSpeed", 8.0f); // Up/down speed
        translateMultiplierStep = HookConfig::GetFloat("Freecam", "TranslateMultiplierStep", 0.075f);
        minTranslateMultiplier = HookConfig::GetFloat("Freecam", "MinTranslateMultiplier", 0.06f);
        maxTranslateMultiplier = HookConfig::GetFloat("Freecam", "MaxTranslateMultiplier", 5.0f);
        rotYSensitivity = HookConfig::GetFloat("Freecam", "RotYSensitivity", 0.0009f);
        rotXSensitivity = HookConfig::GetFloat("Freecam", "RotXSensitivity", 0.0005f);
        minFOV = HookConfig::GetFloat("Freecam", "MinFOV", 5.0f);
        maxFOV = HookConfig::GetFloat("Freecam", "MaxFOV", 120.0f);
        fovStep = HookConfig::GetFloat("Freecam", "FOVStep", 20.0f);

        InstallCallback("Free Cam Handler (1)", "Free camera",
            &SetCameraHook, {
                //cb::call(0x4A6D7E) // Minimap related
                cb::call(0x517C3F), // Cam related
            });

        InstallCallback("Free Cam Handler (2)", "Free camera other",
            &SetCameraOtherHook, {
                cb::call(0x57C36A) // Headlight related
            });

        InstallCallback("Free Cam Handler (4)", "Free camera 3D sound",
            &AddFrameCameraHook, {
                cb::call(0x469C79), // 3D sound related
            });

        InstallCallback("Free Cam Handler (5)", "Free camera PVS position",
            &sub_518FA0_Hook, {
                cb::call(0x524077), // PVS camera related
            });

        InstallCallback("Free Cam Handler (6)", "Free camera SetWorld",
            &SetWorldHook, {
                cb::call(0x520662), // Skybox related
                cb::call(0x5206C3), // Starfield related

            });

        InstallCallback("Free Cam Handler (7)", "Free camera FOV",
            &PerspectiveHook, {
                cb::call(0x57C398), // FOV related
            });

        InstallVTableHook("Cinematic Cam Handler", &ToggleCinematicCamHook, { 0x62D970 });
    }
}
