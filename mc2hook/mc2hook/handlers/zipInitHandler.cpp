#include "zipIintHandler.h"
#include <age\data\args.h>
#include <age\zipfile\zipfile.h>
#include <age\data\asset.h>
#include <age/core/output.h>
#include <age\string\string.h>
#include <age\memory\memory.h>

static int ZipFileSortFunc(const void* a, const void* b)
{
    return strcmp(*(const char**)a, *(const char**)b);
}

void zipInitHandler::zipAutoInit()
{
    // call auto init
    hook::StaticThunk<0x5FDD20>::Call<void>();
}

void zipInitHandler::zipMultiAutoInit(LPCSTR filter)
{
    char FileName[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    memset(&FindFileData, 0, sizeof(FindFileData));
    HANDLE hFindFile;

    strcpy_s(FileName, datAssetManager::GetPath());
    strcat_s(FileName, filter);

    int foundFiles = 0;
    char* foundFileNames[256];

    hFindFile = FindFirstFileA(FileName, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            strcpy_s(FileName, datAssetManager::GetPath());
            strcat_s(FileName, FindFileData.cFileName);
            _strupr_s(FileName);
            foundFileNames[foundFiles] = StringDuplicate(FileName);
            ++foundFiles;
            
        } while (foundFiles != 256 && FindNextFileA(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
    qsort(foundFileNames, foundFiles, 4u, ZipFileSortFunc);

    for (int i = foundFiles - 1; i >= 0; i--)
    {
        auto file = age_new zipFile();
        if (file->Init(foundFileNames[i]))
        {
            datAssetManager::SetPath("");
        }
        else
        {
            delete file;
        }
        age_deallocate(foundFileNames[i]);
    }
}

void zipInitHandler::zipInit()
{
    if (datArgParser::Get("archive"))
    {
        zipAutoInit(); // call into original
    }
    else if(!datArgParser::Get("unpacked"))
    {
        zipMultiAutoInit("*.dat");
    }
}

void zipInitHandler::Install()
{
    InstallCallback("zipInitHandler::zipInit", "Reimplement zipMultiAutoInit.",
        &zipInit, {
            cb::call(0x577491),
        }
    );

    // nop out the -archive check
    mem::nop(0x577480, 5);
    mem::nop(0x577485, 5);
    mem::nop(0x57748A, 3);
    mem::nop(0x57748D, 2);
    mem::nop(0x57748F, 2);
}
