#pragma once
#include <mc2hook\mc2hook.h>

typedef void (*EnumFilesCallback)(LPCSTR, bool, LPVOID);

enum seekWhence {
    seek_begin,
    seek_current,
    seek_end,
};

struct corePagingInfo
{
    LPCSTR name;
    int32_t offset;
    int32_t size;
    int32_t compressedSize;
};

struct coreFileMethods {
    int (*open)(LPCSTR filename, bool readOnly);
    int (*create)(LPCSTR filename);
    int (*read)(int handle, LPVOID buffer, int length);
    int (*write)(int handle, const LPVOID buffer, int length);
    int (*seek)(int handle, int position, seekWhence whence);
    int (*close)(int handle);
    int (*enumFiles)(LPCSTR, EnumFilesCallback, LPVOID);
    int (*size)(int handle);
    int (*flush)(int handle); // usually set to null
    bool (*paging)(LPCSTR filename, corePagingInfo* info, int* handle);
};

static hook::Type<coreFileMethods> coreFileStandard(0x679884);