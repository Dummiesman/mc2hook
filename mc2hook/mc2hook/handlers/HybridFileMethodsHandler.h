#pragma once
#include <mc2hook/mc2hook.h>
#include <age/core/file.h>

struct HybridFileHandle
{
    bool used;
    coreFileMethods* fileMethods;
    int internalHandle;
};

class HybridFileMethodsHandler
{
private:
    static int findFreeHandle();
    static int hybridFileOpen(LPCSTR name, bool readOnly);
    static int hybridFileCreate(LPCSTR name);
    static int hybridFileRead(int handle, LPVOID buffer, int length);
    static int hybridFileWrite(int handle, const LPVOID buffer, int length);
    static int hybridFileSeek(int handle, int offset, seekWhence whence);
    static int hybridFileClose(int handle);
    static int hybridEnumFiles(LPCSTR name, EnumFilesCallback callback, LPVOID userdata);
    static int hybridFileSize(int handle);
    static int hybridFileFlush(int handle);
    static bool hybridFilePaging(LPCSTR filename, corePagingInfo* info, int* handle);
    static coreFileMethods hybridMethods;
public:
    static void Install();
};