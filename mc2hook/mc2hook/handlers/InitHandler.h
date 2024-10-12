#pragma once

// Main handler that hooks game init to install other handlers, which can then access args, filesystem, etc
class InitHandler
{
private:
    static void PreEngineInitHook();
    static void PostEngineInitHook();
public:
    static void Install();
};

