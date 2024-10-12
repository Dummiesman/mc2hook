#include "TestPanelHandler.h"
#include <age\input\keyboard.h>
#include <age/core/output.h>
#include <age/text/stringtable.h>
#include <dinput.h>

void* TestPanelHandler::GetPanelModel(int panelNum, int lod)
{
    if (ioKeyboard::GetKey(DIK_LCONTROL) && ioKeyboard::GetKey(DIK_Q))
    {
        return nullptr;
    }
    return hook::Thunk<0x4E1A80>::Call<void*>(this, panelNum, lod);
}

void* TestPanelHandler::FindBangerType(const char* type)
{
    void* returnPtr = hook::Thunk<0x478850>::Call<void*>(this, type); // mcBangerManager::FindBangerType
    if (returnPtr == nullptr)
    {
        Errorf("mcBangerManager::FindBangerType FAILED on %s", type);
    }
    return returnPtr;
}


void TestPanelHandler::nullsub()
{

}

void TestPanelHandler::Install()
{
#if _DEBUG
    InstallCallback("TestPanelHandler", "Add key to hide panels.",
        &GetPanelModel, {
            cb::call(0x4C938C),
            cb::call(0x4CB4E5),
        }
    );

    InstallCallback("FindBangerType", "Debug missing bangers.",
        &FindBangerType, {
            cb::call(0x478FEB),
            cb::call(0x478F6F),
            cb::call(0x478DC2),
            cb::call(0x478C11),
        }
 
   );
#endif
}
