#pragma once
#include <mc2hook\mc2hook.h>

class txtFontTexture
{
private:
    uint8_t buf[0x33C];
protected:
    static hook::Field<0x338, int32_t> _refCount;
public:
    void IncRef()
    {
        _refCount.set(this, _refCount.get(this) + 1);
    }

    void DecRef()
    {
        _refCount.set(this, _refCount.get(this) - 1);
    }
};