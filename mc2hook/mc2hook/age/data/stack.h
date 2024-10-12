#pragma once
#include <mc2hook\mc2hook.h>

// Forward declarations
class datStack;
struct MapSymbol;

// External declarations


// Class definitions
struct MapSymbol
{
    const char* Name;
    int Address;
};

class datStack {
private:
    static hook::Type<bool> MapInitialized;
    static hook::Type<int> MapSymbolCount;
    static hook::Type<char[128]> MapTimestamp;
    static hook::Type<MapSymbol*> MapSymbols;
private:
    static void TracebackLua(int length);
    static std::string LookupAddressLua(int address);
public:
    // marked public for hooks
    static int ExceptionFilter(EXCEPTION_POINTERS* eptrs);
    static int ExceptionFilterWithMsgBox(EXCEPTION_POINTERS* eptrs);
    static int ExceptionFilterCombined(EXCEPTION_POINTERS* eptrs);
public:
    static char const* GetTimestamp();
    static void LookupAddress(char* buf, int address);
    static void DoTraceback(int length, DWORD* contextRecordEBP, FILE* output, char const* lineSeperator);
    static void Traceback(int length, FILE* output);

    static const MapSymbol* LookupMapSymbol(int address);
    static void InitDebugSymbols();
};