#include "CarShadowHandler.h"

static float carShadowIntensity;

void CarShadowHandler::Install()
{
    carShadowIntensity = HookConfig::GetFloat("Graphics", "CarShadowIntensity", 0.5f);

    mem::write(0x4CAF05 + 2, &carShadowIntensity);
}
