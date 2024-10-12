#include "StateResearchHook.h"
#include <age/core/output.h>
#include <mc2hook\mc2hook.h>

void StateResearchHook::EnterState(int num)
{
    for (int i = 0; i < 4; i++) Warningf("==============================");
    Warningf("== StateResearchHook::EnterState(%i) ==", num);
    for (int i = 0; i < 4; i++) Warningf("==============================");
    hook::Thunk<0x404A90>::Call<void>(this, num);
}

void StateResearchHook::SwitchStateMaybe(int num)
{
    for (int i = 0; i < 4; i++) Warningf("==============================");
    Warningf("== StateResearchHook::SwitchStateMaybe(%i) ==", num);
    for (int i = 0; i < 4; i++) Warningf("==============================");
    hook::Thunk<0x403810>::Call<void>(this, num);
}

void StateResearchHook::Install()
{
#if _DEBUG
    InstallVTableHook("SRH1", &SwitchStateMaybe, { 0x62D96C });
    InstallVTableHook("SRH2", &EnterState, { 0x62D978 });
    InstallCallback("SRH3", &EnterState, { cb::call(0x404BAE) } );
#endif
}
