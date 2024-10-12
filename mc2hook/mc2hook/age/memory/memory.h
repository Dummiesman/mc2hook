#pragma once
#include <mc2hook\mc2hook.h>


#define __MEMTRACEFILE__ __BASE_FILE__

#define age_new new(__MEMTRACEFILE__, __LINE__)
#define age_allocate(size)  hook::StaticThunk<0x5772E0>::Call<void*>(size, __MEMTRACEFILE__, __LINE__);
#define age_delete operator delete(__MEMTRACEFILE__, __LINE__)
#define age_deallocate(ptr) hook::StaticThunk<0x577320>::Call<void>(ptr) 