#include "ReflectionFidelityHandler.h"

void ReflectionFidelityHandler::Install()
{
    int reflectionResolution = HookConfig::GetInt("Graphics", "ReflectionResolution", 128);
    bool reflectionFrameSkip = HookConfig::GetBool("Graphics", "ReflectionFrameSkip", true);

    mem::write(0x52299E + 6, reflectionResolution); // mcEnvMap::m_CubemapResolution

    if (!reflectionFrameSkip)
    {
        mem::write(0x522B0C, static_cast<uint8_t>(0xEB)); // Change JZ to JMP for mcEnvMap::sm_FirstFrame check
                                                          // (forces it to always evaluate to true and render the entire cubemap each frame)
    }
}
