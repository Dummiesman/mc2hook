#pragma once
#include <mc2hook\mc2hook.h>
#include <age/core/file.h>

class datAssetManager
{
private:
    void* vtbl;
protected:
    static hook::Type<char[256]> sm_Path;
public:
    bool Exists(LPCSTR dir, LPCSTR file, LPCSTR extension)
    {
        return hook::Thunk<0x613880>::Call<bool>(this, dir, file, extension);
    }

    int EnumFiles(LPCSTR path, EnumFilesCallback callback, void* this_pointer, bool useCore)
    {
        return hook::Thunk<0x6138B0>::Call<int>(this, path, callback, this_pointer, useCore);
    }

    static void SetPath(LPCSTR path)
    {
        hook::StaticThunk<0x6134D0>::Call<void>(path);
    }

    static LPCSTR GetPath()
    {
        return sm_Path.ptr();
    }
};

static hook::Type<datAssetManager*> ASSETP = 0x860220;