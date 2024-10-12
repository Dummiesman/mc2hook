#include "dllmain.h"
#include <framework\hook_framework.h>
#include <handlers\handlers.h>
#include <mc2hook\mc2hook_defs.h>

LPFNDIRECTINPUTCREATE lpDICreate;

#if DIRECTINPUT_VERSION == 0x0700
// Export as 'DirectInputCreateA/W' so we can hook into the application
// (Apparently DirectInputCreateW gets called sometimes...)
#pragma comment(linker, "/EXPORT:DirectInputCreateA=_DirectInputCreateImpl")
#pragma comment(linker, "/EXPORT:DirectInputCreateW=_DirectInputCreateImpl")
HRESULT __declspec(naked) DirectInputCreateImpl(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppDI, LPUNKNOWN punkOuter)
{
    _asm jmp dword ptr ds : lpDICreate
}
#define HOOK_HOOKED_LIBRARY "dinput.dll"
#define HOOK_HOOKED_PROC "DirectInputCreateA"
#elif DIRECTINPUT_VERSION == 0x0800
// Export as 'DirectInput8Create' so we can hook into the application
#pragma comment(linker, "/EXPORT:DirectInput8Create=_DirectInputCreateImpl")
HRESULT __declspec(naked) DirectInputCreateImpl(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    _asm jmp dword ptr ds : lpDICreate
}
#define HOOK_HOOKED_LIBRARY "dinput8.dll"
#define HOOK_HOOKED_PROC "DirectInput8Create"
#endif

bool LoadSystemLibrary(LPCSTR lpDllName, HMODULE* out)
{
    char szDllFile[MAX_PATH]{ NULL }; // path to system dll

    GetSystemDirectory(szDllFile, sizeof(szDllFile));
    sprintf(szDllFile, "%s\\%s", szDllFile, lpDllName);

    if (SUCCEEDED(*out = LoadLibrary(szDllFile)))
    {
        printf("Loaded system library \"%s\" => %08X\n", szDllFile, *out);
        return true;
    }
    else
    {
        printf("System library \"%s\" not found!\n", szDllFile);
        return false;
    }
};

bool GetHookProcAddress(HMODULE hModule, LPCSTR lpProcName, FARPROC* out)
{
    if (SUCCEEDED(*out = GetProcAddress(hModule, lpProcName)))
    {
        printf("Found hook proc \"%s\" in module %08X => %08X\n", lpProcName, hModule, *out);
        return true;
    }
    else
    {
        printf("Hook proc \"%s\" not found in module %08X!\n", lpProcName, hModule);
        return false;
    }
};

static void Initialize()
{
    HookConfig::Init();

    bool useConsole = HookConfig::GetBool("Debug", "ShowConsole", false);
    bool useLog = HookConfig::GetBool("Debug", "EnableLogFile", false);

    int outputFlags = 0;
    outputFlags |= (useConsole) ? HOOK_OUTPUT_CONSOLE : 0;
    outputFlags |= (useLog) ? HOOK_OUTPUT_LOGFILE : 0;
    InitOutput(outputFlags);
    atexit(KillOutput);

    hook_output("%s, compiled on %s %s", PRODUCT_NAME, __DATE__, __TIME__);
    InstallMainHandlers();
}

bool CheckGameValid()
{
    return true; //TODO
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    HMODULE hDIModule = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        printf("Initting %s", PRODUCT_NAME);

        if (CheckGameValid())
        {
            if (LoadSystemLibrary(HOOK_HOOKED_LIBRARY, &hDIModule) &&
                GetHookProcAddress(hDIModule, HOOK_HOOKED_PROC, (FARPROC*)&lpDICreate))
            {
                Initialize();
            }
            else
            {
                MessageBox(NULL, "Could not inject into the game process. Unknown errors may occur.", PRODUCT_NAME, MB_OK | MB_ICONWARNING);
            }
        }
        else
        {
            MessageBox(NULL, "Did not detect a supported version of the game.", PRODUCT_NAME, MB_OK | MB_ICONWARNING);
            ExitProcess(EXIT_FAILURE);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}