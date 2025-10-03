#include "CarShadowHandler.h"

void CarShadowHandler::SetColor_Hook(float a1)
{
    hook::StaticThunk<0x5EE630>::Call<void>(0.0f); // Call original with a custom paramater
}

void CarShadowHandler::Install()
{
    bool applyCarShadowOpacity = HookConfig::GetBool("Graphics", "ApplyCarShadowOpacity", true);

    if (!applyCarShadowOpacity)
    {
        InstallCallback("Car Shadow Handler", "Car shadow opacity control",
            &SetColor_Hook, {
                cb::call(0x4CAF23)
            });
    }
}
