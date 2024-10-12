#pragma once
#include <mc2hook\mc2hook.h>

extern _Type<void(*)(int, LPCSTR, va_list)> sDefaultPrinter;
extern _Type<uint32_t> sOutputMask;

extern void Printf(LPCSTR str, ...);
extern void Messagef(LPCSTR str, ...);
extern void Displayf(LPCSTR str, ...);
extern void Warningf(LPCSTR str, ...);
extern void Errorf(LPCSTR str, ...);
extern void Quitf(LPCSTR str, ...);
extern void Abortf(LPCSTR str, ...);

class datOutput
{
public:
    static hook::Type<bool> OutputSent;
    static hook::Type<bool> ShowPopupErrors;
    static hook::Type<bool> ShowPopupQuits;
public:
    static void SetOutputMask(uint32_t mask)
    {
        hook::StaticThunk<0x618490>::Call<void>(mask);
    }

    static void CallBeforeMsgBoxFunction(void) {
        hook::StaticThunk<0x618470>::Call<void>();
    }

    static void CallAfterMsgBoxFunction(void) {
        hook::StaticThunk<0x618480>::Call<void>();
    }
};