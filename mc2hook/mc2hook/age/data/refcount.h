#pragma once
#include <mc2hook\mc2hook.h>

class datRefCount
{
private:
    int32_t m_Count;
public:
    datRefCount()
    {
        m_Count = 0;
    }

    virtual ~datRefCount()
    {
    }

    virtual void IncRef() 
    {
        m_Count++; 
    }

    virtual void DecRef() 
    {
        m_Count--; 
        if (m_Count == 0)
        {
            delete this;
        }
    }
};

#define ASSIGN_REF(dst, value) auto currentValue = dst; \
                              if(currentValue != value) { \
                                if(dst) dst->DecRef(); \
                                dst = value; \
                                if(dst) dst->IncRef(); \
                              }