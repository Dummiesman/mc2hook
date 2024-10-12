#include "zipEnumHandler.h"
#include <age/zipfile/zipfile.h>
#include <age/core/output.h>

/// Angel never updated EnumFiles to address compressed names
/// We deal with that here

static void __zipRecurseExpand(char* buf, const char** name)
{
    hook::StaticThunk<0x5FD300>::Call<void>(buf, name);
}

void zipEnumHandler::EnumFiles(LPCSTR path, void(__cdecl* cb)(const char*, bool, void*), void* userdata)
{
    auto zip = reinterpret_cast<zipFile*>(this);

    // get path root
    char pathRoot[256];
    strcpy_s(pathRoot, path);
    for (; strchr(pathRoot, '\\'); *strchr(pathRoot, '\\') = 0);

    int pathRootLen = strlen(pathRoot);
    if (pathRootLen > 0 && pathRoot[pathRootLen] != '/')
    {
        pathRoot[pathRootLen] = '/';
        pathRootLen++;
    }

    // enum over files
    auto entries = zip->GetEntries();
    char outBuf[1024] = { 0 };
    for (int i = 0; i < zip->GetNumEntries(); i++)
    {
        auto entry = &entries[i];
        if (zipFile::s_8to6CompressNameHeap.get())
        {
            if (zipFile::s_PrefixCompressNameHeap.get())
            {
                zipFile::zipRecurseExpand(outBuf, &entry->Name);
            }
            else
            {
                // Uh oh, this is unimplemented (but afaik, unused)
                Quitf("DM: Unsupported EnumFiles compression combination: s_8to6CompressNameHeap && s_PrefixCompressNameHeap == s_8to6CompressNameHeap.");
            }
        }
        else
        {
            strcpy_s(outBuf, entry->Name);
        }

        if (!_strnicmp(outBuf, pathRoot, pathRootLen))
        {
            cb(pathRootLen + outBuf, false, userdata);
        }
    }
}

void zipEnumHandler::Install()
{
    InstallCallback("zipInitHandler::EnumFiles", "Reimplement zipFile::EnumFiles.",
        &EnumFiles, {
            cb::call(0x5FD2B1),
        }
    );
}
