#include "output.h"

#define DEFINE_PRINT_HOOK(x) \
__declspec(naked) void x(LPCSTR str, ...) { \
    __asm jmp dword ptr ds:$##x \
}

_Type<void(*)(int, LPCSTR, va_list)> sDefaultPrinter(0x679880);
_Type<uint32_t> sOutputMask(0x679848);

_Func<void> $Printf(0x618610);
_Func<void> $Messagef(0x618630);
_Func<void> $Displayf(0x618650);
_Func<void> $Warningf(0x618670);
_Func<void> $Errorf(0x618690);
_Func<void> $Quitf(0x6186B0);
_Func<void> $Abortf(0x618700);

DEFINE_PRINT_HOOK(Printf);
DEFINE_PRINT_HOOK(Messagef);
DEFINE_PRINT_HOOK(Displayf);
DEFINE_PRINT_HOOK(Warningf);
DEFINE_PRINT_HOOK(Errorf);
DEFINE_PRINT_HOOK(Quitf);
DEFINE_PRINT_HOOK(Abortf);

declfield(datOutput::OutputSent)(0x86D298);
declfield(datOutput::ShowPopupErrors)(0x86D288);
declfield(datOutput::ShowPopupQuits)(0x679844);