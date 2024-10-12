#include "timemgr.h"

declfield(datTimeManager::Seconds)(0x6797CC);
declfield(datTimeManager::InvSeconds)(0x6797D4);
declfield(datTimeManager::ActualSeconds)(0x6797D0);
declfield(datTimeManager::ElapsedTime)(0x8602E0);
declfield(datTimeManager::PrevElapsedTime)(0x8602D8);
declfield(datTimeManager::FPS)(0x6797D8);
declfield(datTimeManager::UnwarpedSeconds)(0x6797F4);

float datTimeManager::GetSeconds()
{
    return datTimeManager::Seconds.get();
}

float datTimeManager::GetInvSeconds()
{
    return datTimeManager::InvSeconds.get();
}

float datTimeManager::GetActualSeconds()
{
    return datTimeManager::ActualSeconds.get();
}

float datTimeManager::GetElapsedTime()
{
    return datTimeManager::ElapsedTime.get();
}

float datTimeManager::GetPrevElapsedTime()
{
    return datTimeManager::PrevElapsedTime.get();
}

float datTimeManager::GetFPS()
{
    return datTimeManager::FPS.get();
}

float datTimeManager::GetUnwarpedSeconds()
{
    return datTimeManager::UnwarpedSeconds.get();
}
