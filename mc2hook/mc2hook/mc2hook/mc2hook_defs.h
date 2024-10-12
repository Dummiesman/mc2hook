#pragma once
#include <stdint.h>

static const char* PRODUCT_NAME = "Midnight Club 2 Hook";

#define mc2hook_assert(cond) if(!(cond)) { MessageBox(NULL, #cond, "Assertion failed", MB_OK | MB_ICONERROR); ExitProcess(EXIT_FAILURE); }
//#define MC2HOOK_DEBUG_FEATURES

enum GameType
{
    Game_Unknown = -1,
    Game_AM2,
    Game_AM2_Steam
};

struct GameTypeInfo
{
    const char* Name;
    const uint16_t CRC;
    const GameType Type;
};

const int AM2HOOK_SupportedGameCount = 2;
static GameTypeInfo AM2HOOK_SupportedGames[AM2HOOK_SupportedGameCount] =
{
    {
        "Army Men 2",
        0x551b,
        GameType::Game_AM2
    },
    {
        "Army Men 2 (Steam)",
        0xf75b,
        GameType::Game_AM2_Steam
    },
};

static GameTypeInfo* AM2HOOK_Current_Game = nullptr;