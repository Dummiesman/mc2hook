#pragma once
#include <age\ui\widget.h>

class mcUiIconSlot
{
public:
    int32_t dword_04;
    int32_t index;
public:
    mcUiIconSlot()
    {
        this->dword_04 = -1;
        this->index = -1;
    }

    virtual ~mcUiIconSlot()
    {

    }

    int GetIndex() const
    {
        return index;
    }
};

class mcUiIconBase : public uiWidget
{
    int32_t dword_48;
    int32_t dword_4c;
    int32_t dword_50;
    int32_t dword_54;
    int32_t dword_58;
    int32_t dword_5c;
    int32_t dword_60;
    int32_t dword_64;
    int32_t dword_68;
    int32_t dword_6c;
    int32_t dword_70;
    int32_t dword_74;
    int32_t dword_78;
};

ASSERT_SIZEOF(mcUiIconBase, 0x7C);