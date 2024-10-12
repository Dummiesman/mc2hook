#pragma once
#include <Windows.h>
#include <stdio.h>
#include "hook.h"

#define HOOK_OUTPUT_BUF_SIZE 4096
#define HOOK_OUTPUT_CONSOLE 1
#define HOOK_OUTPUT_LOGFILE 2

class HookConsoleOutput
{
private:
    static HANDLE hConsole;
    static CONSOLE_SCREEN_BUFFER_INFO cInfo;
public:
    static void Print(LPCSTR format, ...);
    static void Init();
    static void Close();
};

class HookLogOutput
{
private:
    static FILE* fStream;
public:
    static void Print(LPCSTR format, ...);
    static void Init();
    static void Close();
};

static void hook_output(LPCSTR format, ...) 
{
    char buf[HOOK_OUTPUT_BUF_SIZE] = { 0 };

    va_list va;
    va_start(va, format);
    vsprintf_s(buf, format, va);
    va_end(va);

    // actually output
    printf("%s\n", buf);
    HookConsoleOutput::Print("%s\n", buf);
    HookLogOutput::Print("%s\n", buf);
}

static void hook_output_raw(LPCSTR format, ...)
{
    char buf[HOOK_OUTPUT_BUF_SIZE] = { 0 };

    va_list va;
    va_start(va, format);
    vsprintf_s(buf, format, va);
    va_end(va);

    // actually output
    printf("%s", buf);
    HookConsoleOutput::Print("%s", buf);
    HookLogOutput::Print("%s", buf);
}

static void InitOutput(int flags) 
{
    if(flags & HOOK_OUTPUT_CONSOLE)
        HookConsoleOutput::Init();
    if (flags & HOOK_OUTPUT_LOGFILE)
        HookLogOutput::Init();
}

static void KillOutput()
{
    HookConsoleOutput::Close();
    HookLogOutput::Close();
}

