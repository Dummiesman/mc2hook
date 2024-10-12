#include "hook_output.h"

HANDLE HookConsoleOutput::hConsole = nullptr;
CONSOLE_SCREEN_BUFFER_INFO HookConsoleOutput::cInfo;

FILE* HookLogOutput::fStream = nullptr;

/*
* HookConsoleOutput
*/
void HookConsoleOutput::Print(LPCSTR format, ...)
{
    if (hConsole == nullptr)
        return;

    char buf[HOOK_OUTPUT_BUF_SIZE] = { 0 };

    va_list va;
    va_start(va, format);
    int bufLen = vsprintf_s(buf, format, va);
    va_end(va);

    // actually output
    DWORD discard;
    WriteConsole(hConsole, buf, bufLen, &discard, NULL);
}

void HookConsoleOutput::Init()
{
    if (hConsole != nullptr) 
    {
        printf("Tried to allocate more than one console!\n");
    }
    else 
    {
        AllocConsole();
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleScreenBufferInfo(hConsole, &cInfo);
        cInfo.dwSize.Y = 2500;
        SetConsoleScreenBufferSize(hConsole, cInfo.dwSize);

        SetConsoleTitle("Hook Console");
    }
}

void HookConsoleOutput::Close()
{
    if (hConsole != nullptr) 
    {
        FreeConsole();
        hConsole = nullptr;
    }
}

/*
* HookLogOutput
*/
void HookLogOutput::Print(LPCSTR format, ...)
{
    if (fStream == nullptr)
        return;

    

    char buf[HOOK_OUTPUT_BUF_SIZE] = { 0 };

    va_list va;
    va_start(va, format);
    int bufLen = vsprintf_s(buf, format, va);
    va_end(va);

    // actually output
    fprintf(fStream, "%s", buf);
    fflush(fStream);
}

void HookLogOutput::Init()
{
    if (fStream != nullptr)
    {
        printf("Tried to open more than one logfile!\n");
    }
    else
    {
        fStream = fopen("am2hook.log", "w");
    }
}

void HookLogOutput::Close()
{
    if (fStream != nullptr)
    {
        fclose(fStream);
        fStream = nullptr;
    }
}