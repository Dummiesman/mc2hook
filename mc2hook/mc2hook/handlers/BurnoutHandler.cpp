#include "BurnoutHandler.h"
#include <age/data/timemgr.h>

static float burnoutIncreaseSpeed = 0.0f;
static float burnoutDecreaseSpeed = 0.0f;
static float burnoutDamageAmount = 0.0f;

static float burnoutIncreaseSpeedCached = 0.0f;
static float burnoutDecreaseSpeedCached = 0.0f;
static float burnoutDamageAmountCached = 0.0f;

static float physicsFixesBaselineFPS = 0.0f;

static bool vehCarSimValuesCached = false;

void BurnoutHandler::vehCarSimUpdate()
{
    hook::Thunk<0x4D3290>::Call<void>(this); // call original

    if (vehCarSimValuesCached == false)
    {
        burnoutIncreaseSpeedCached = *getPtr<float>(this, 0x190);
        burnoutDecreaseSpeedCached = *getPtr<float>(this, 0x194);
        burnoutDamageAmountCached = *getPtr<float>(this, 0x198);
        vehCarSimValuesCached = true;
    }

    burnoutIncreaseSpeed = burnoutIncreaseSpeedCached * datTimeManager::GetSeconds() * physicsFixesBaselineFPS;
    burnoutDecreaseSpeed = burnoutDecreaseSpeedCached * datTimeManager::GetSeconds() * physicsFixesBaselineFPS;
    burnoutDamageAmount = burnoutDamageAmountCached * datTimeManager::GetSeconds() * physicsFixesBaselineFPS;

    *getPtr<float>(this, 0x190) = burnoutIncreaseSpeed;
    *getPtr<float>(this, 0x194) = burnoutDecreaseSpeed;
    *getPtr<float>(this, 0x198) = burnoutDamageAmount;
}

void BurnoutHandler::Install()
{
    physicsFixesBaselineFPS = HookConfig::GetFloat("Physics", "PhysicsFixesBaselineFPS", 30.0f);

    // Cap physics at 30fps as a lowest possible value
    if (physicsFixesBaselineFPS < 30.0f)
    {
        physicsFixesBaselineFPS = 30.0f;
    }

    InstallVTableHook("vehCarSim Some Update", &vehCarSimUpdate, { 0x644A6C });
}
