#include "allocator.h"

declfield(memMemoryAllocator::sm_Current)(0x6C5258);

memMemoryAllocator* memMemoryAllocator::GetCurrent()
{
    return sm_Current.get();
}
