#pragma once
#include "widget.h"

class uiGroupBase : public uiWidget
{
    /* uiMaster* */ void* pMaster;
    int dword_4c;
};

class uiGroup : public uiGroupBase
{
};