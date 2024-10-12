#pragma once

class AGEAllocatedClass
{
public:
    void* operator new(size_t size, const char* file, int line);
    void* operator new[](size_t size, const char* file, int line);

    void operator delete(void* ptr, const char* file, int line);
    void operator delete[](void* ptr, const char* file, int line);

    void operator delete(void* ptr);
};