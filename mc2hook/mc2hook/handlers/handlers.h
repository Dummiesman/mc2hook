#pragma once
#include <handlers\InitHandler.h>
#include <handlers\zipIintHandler.h>
#include <handlers\CustomExceptionHandler.h>
#include <handlers\PrinterHandler.h>
#include <handlers\FontCrashFixHandler.h>
#include <handlers\RemoveForcedArgsHandler.h>
#include <handlers\OpenSpyHandler.h>
#include <handlers\ChatHandler.h>

#include <handlers\CustomVehicleHandler.h>
#include <handlers\SteeringSmootherHandler.h>
#include <handlers\PathHandler.h>
#include <handlers\zipEnumHandler.h>
#include <handlers\TestPanelHandler.h>
#include <handlers\vehTransmisisonHandler.h>
#include <handlers\InputHandler.h>
#include <handlers\BorderlessHandler.h>
#include <handlers\ReflectionFidelityHandler.h>

#include <handlers\StateResearchHook.h>

// Installed before engine init
static void InstallHandlersPreEngineInit()
{
    InstallHandler<InputHandler>("Input Handler");
    InstallHandler<BorderlessHandler>("Borderless Handler");
}

// Installed after engine init
static void InstallHandlersPostEngineInit()
{
    InstallHandler<CustomVehicleHandler>("Custom Vehicle Handler");
    InstallHandler<SteeringSmootherHandler>("Smooth Steering");
    InstallHandler<PathHandler>("Path Handler");
    InstallHandler<TestPanelHandler>("Panel Handler (Keyboard input test really)");
    InstallHandler<vehTransmissionHandler>("Transmission Handler");
}

// Installed at game launch
static void InstallMainHandlers()
{
    InstallHandler<CustomExceptionHandler>("Custom Exception Handler");
    InstallHandler<PrinterHandler>("Printer Handler");
    InstallHandler<zipInitHandler>("ZIP Handler");
    InstallHandler<zipEnumHandler>("ZIP Enum Handler");
    InstallHandler<RemoveForcedArgsHandler>("Remove Forced Args Handler");
    InstallHandler<FontCrashFixHandler>("Font Crash Fix");
    InstallHandler<OpenSpyHandler>("OpenSpy Handler");
    InstallHandler<ChatHandler>("Chat Handler");
    InstallHandler<InitHandler>("Game Init Handler");
    InstallHandler<ReflectionFidelityHandler>("Reflection Fidelity Handller");

    InstallHandler<StateResearchHook>("SRH");
}