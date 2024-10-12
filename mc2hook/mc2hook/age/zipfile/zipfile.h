#pragma once
#include <mc2hook\mc2hook.h>
#include <age\memory\age_alloc_baseclass.h>
#include <age/core/file.h>

struct zipEntry
{
    const char* Name;
    int32_t dword_04;
    int32_t dword_08;
    int32_t dword_0c;
};


class zipFile : public AGEAllocatedClass
{
public:
    static hook::Type<bool> s_8to6CompressNameHeap;
    static hook::Type<bool> s_PrefixCompressNameHeap;
    static hook::Type<coreFileMethods> sm_ZipMethods;
private:
    uint8_t buffer[0x20];
protected:
    static hook::Field<0x10, zipEntry*> _entries;
    static hook::Field<0x14, int32_t> _numEntries;
public:
    static void zipRecurseExpand(char* buf, const char** str)
    {
        hook::StaticThunk<0x5FD300>::Call<void>(buf, str);
    }
public:
    zipFile() { hook::Thunk<0x5FC660>::Call<void>(this); }
    ~zipFile() { hook::Thunk<0x5FC690>::Call<void>(this); }

    bool Init(LPCSTR file)
    {
        return hook::Thunk<0x5FDCE0>::Call<bool>(this, file);
    }

    zipEntry* Locate(LPCSTR file)
    {
        return hook::Thunk<0x5FD8B0>::Call<zipEntry*>(this, file);
    }

    zipEntry* GetEntries()
    {
        return _entries.get(this);
    }

    int32_t GetNumEntries() const
    {
        return _numEntries.get(this);
    }
};
ASSERT_SIZEOF(zipFile, 0x20);