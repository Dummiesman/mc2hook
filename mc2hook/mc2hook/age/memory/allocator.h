#pragma once
#include <mc2hook\mc2hook.h>

class memMemoryAllocator
{
private:
    static hook::Type<memMemoryAllocator*> sm_Current;
public:
    void* Allocate(size_t size, bool a2, LPCSTR sourceFile, int sourceFileLine) 
    {
        return hook::Thunk<0x576B10>::Call<void*>(this, size, a2, sourceFile, sourceFileLine);
    }
        
    static memMemoryAllocator* GetCurrent();
};