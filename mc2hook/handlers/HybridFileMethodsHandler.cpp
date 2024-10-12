#include "HybridFileMethodsHandler.h"
#include <age/zipfile/zipfile.h>
#include <age/core/file.h>
#include <age/core/output.h>

/*
* WORK IN PROGRESS
* Until mc2hook came along, modders unpacked their games
* For people who are disk IO limited, it's better to use .dat archives
* Currently these two methods cannot be used together
* That's the goal of this class
*/
const int HYBRID_HANDLE_COUNT = 64;
const int HYBRID_HANDLE_INVALID = -1;
HybridFileHandle handles[HYBRID_HANDLE_COUNT];

int HybridFileMethodsHandler::findFreeHandle()
{
    for (int i = 0; i < HYBRID_HANDLE_COUNT; i++)
    {
        if (!handles[i].used) return i;
    }
    return HYBRID_HANDLE_INVALID;
}

int HybridFileMethodsHandler::hybridFileOpen(LPCSTR name, bool readOnly)
{
    int handle = findFreeHandle();
    if (handle == HYBRID_HANDLE_INVALID)
    {
        Errorf("hybridFileOpen(%s): Out of handles!", name);
        return handle;
    }

    // try open from disk
    int internalHandle = coreFileStandard->open(name, readOnly);
    if (internalHandle)
    {
        handles[handle].used = true;
        handles[handle].fileMethods = &coreFileStandard;
        handles[handle].internalHandle = internalHandle;
        return handle;
    }

    // try open from zipfile
    internalHandle = zipFile::sm_ZipMethods->open(name, readOnly);
    if (internalHandle)
    {
        handles[handle].used = true;
        handles[handle].fileMethods = zipFile::sm_ZipMethods.ptr();
        handles[handle].internalHandle = internalHandle;
        return handle;
    }

    return HYBRID_HANDLE_INVALID;
}

int HybridFileMethodsHandler::hybridFileCreate(LPCSTR name)
{
    // create only works on core, so I guess we'll forward this
    int handle = findFreeHandle();
    if (handle == HYBRID_HANDLE_INVALID)
    {
        Errorf("hybridFileCreate(%s): Out of handles!", name);
        return handle;
    }

    handles[handle].used = true;
    handles[handle].fileMethods = &coreFileStandard;
    handles[handle].internalHandle = coreFileStandard->create(name);
    return handle;
}

int HybridFileMethodsHandler::hybridFileRead(int handle, LPVOID buffer, int length)
{
    mc2hook_assert(handles[handle].used);
    return handles[handle].fileMethods->read(handles[handle].internalHandle, buffer, length);
}

int HybridFileMethodsHandler::hybridFileWrite(int handle, const LPVOID buffer, int length)
{
    mc2hook_assert(handles[handle].used);
    return handles[handle].fileMethods->write(handles[handle].internalHandle, buffer, length);
}

int HybridFileMethodsHandler::hybridFileSeek(int handle, int offset, seekWhence whence)
{
    mc2hook_assert(handles[handle].used);
    return handles[handle].fileMethods->seek(handles[handle].internalHandle, offset, whence);
}

int HybridFileMethodsHandler::hybridFileClose(int handle)
{
    mc2hook_assert(handles[handle].used);
    handles[handle].used = false; // free my boy
    return handles[handle].fileMethods->close(handles[handle].internalHandle);
}

int HybridFileMethodsHandler::hybridEnumFiles(LPCSTR name, EnumFilesCallback callback, LPVOID userdata)
{
    int count = 0;
    count += coreFileStandard->enumFiles(name, callback, userdata);
    count += zipFile::sm_ZipMethods->enumFiles(name, callback, userdata);
    return count;
}

int HybridFileMethodsHandler::hybridFileSize(int handle)
{
    mc2hook_assert(handles[handle].used);
    return handles[handle].fileMethods->size(handle);
}

int HybridFileMethodsHandler::hybridFileFlush(int handle)
{
    mc2hook_assert(handles[handle].used);
    return handles[handle].fileMethods->flush(handle);
}

bool HybridFileMethodsHandler::hybridFilePaging(LPCSTR filename, corePagingInfo* info, int* handle)
{
    // only zip files have paging support
    // therefore this method would only get called on a zip file
    // the OUT handle is a system file handle, we can just forward this
    return zipFile::sm_ZipMethods->paging(filename, info, handle);
}

coreFileMethods HybridFileMethodsHandler::hybridMethods =
{
    &hybridFileOpen,
    &hybridFileCreate,
    &hybridFileRead,
    &hybridFileWrite,
    &hybridFileSeek,
    &hybridFileClose,
    &hybridEnumFiles,
    &hybridFileSize,
    &hybridFileFlush,
    &hybridFilePaging
};

void HybridFileMethodsHandler::Install()
{
}
