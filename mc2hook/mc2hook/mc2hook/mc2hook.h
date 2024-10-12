#pragma once
#include "mc2hook_defs.h"
#include "mc2hook_util.h"
#include <framework/hook_framework.h>
#include <stdint.h>
#include <string.h>

//template <typename THandler>
//static void InstallHandler(const char* name) {
//    bool gSupport = THandler::IsGameSupported(AM2HOOK_Current_Game->Type);
//    if (gSupport)
//    {
//        static void(*lpInstall)(GameType) = &THandler::Install;
//        lpInstall(AM2HOOK_Current_Game->Type);
//    }
//}

template <typename THandler>
static void InstallHandler(const char* name) {
    static void(*lpInstall)() = &THandler::Install;
    lpInstall();
}