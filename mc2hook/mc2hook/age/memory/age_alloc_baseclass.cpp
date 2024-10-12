#include "age_alloc_baseclass.h"
#include <mc2hook\mc2hook.h>

void* AGEAllocatedClass::operator new(size_t size, const char* file, int line)
{
    return hook::StaticThunk<0x5772E0>::Call<void*>(size, file, line);
}

void AGEAllocatedClass::operator delete(void* ptr, const char* file, int line)
{
    hook::StaticThunk<0x577320>::Call<void>(ptr);
}

void AGEAllocatedClass::operator delete(void* ptr)
{
    hook::StaticThunk<0x577320>::Call<void>(ptr);
}

