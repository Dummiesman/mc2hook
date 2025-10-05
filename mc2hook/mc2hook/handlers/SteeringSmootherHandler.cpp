#include "SteeringSmootherHandler.h"
#include <age/data/timemgr.h>
#include <age/core/output.h>
#include <age/data/args.h>

static float lastRequestedSteering = 0.0f;
static float steerValue = 0.0f;

static float steerSpeed = 4.0f;
static float steerSpeedOut = 6.0f;

float controllerSteerSpeed;

void SteeringSmootherHandler::Update()
{
    hook::Thunk<0x46B330>::Call<void>(this); // call original
    
    // TODO: Make this use a discrete filter like Midtown Madness 2?
    // Also look into a better way of doing it
    // Also also check if the input device is digital (KB)
    lastRequestedSteering = *getPtr<float>(this, 0x1C);
    if (lastRequestedSteering > steerValue)
    {

        float speed = (lastRequestedSteering >= 0 && steerValue < 0) ? steerSpeedOut : steerSpeed;
        steerValue += datTimeManager::GetSeconds() * speed;
        if (steerValue > lastRequestedSteering) steerValue = lastRequestedSteering;
    }
    else if (lastRequestedSteering < steerValue)
    {
        float speed = (lastRequestedSteering <=0 && steerValue > 0) ? steerSpeedOut : steerSpeed;
        steerValue -= datTimeManager::GetSeconds() * speed;
        if (steerValue < lastRequestedSteering) steerValue = lastRequestedSteering;
    }
    else
    {
        steerValue = lastRequestedSteering;
    }

    *getPtr<float>(this, 0x1C) = steerValue;
}

void SteeringSmootherHandler::Install()
{
    if (datArgParser::Get("smoothsteer")) {
        InstallVTableHook("Input Update", &Update, { 0x63D0C4 });
    }

    // Exposing controller speed value
    controllerSteerSpeed = HookConfig::GetFloat("Input", "ControllerSteerSpeed", 10.0f);
    mem::write(0x46B28E + 3, static_cast<float>(controllerSteerSpeed));
}
