#include "CityModelLimitHandler.h"

void CityModelLimitHandler::Install()
{
    mem::nop(0x4691E0, 7); // -nopvs crash fix
}
