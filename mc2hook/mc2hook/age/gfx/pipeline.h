#pragma once
#include <mc2hook\mc2hook.h>

class gfxPipeline
{
public:
    static hook::TypeProxy<int> m_iWidth;
    static hook::TypeProxy<int> m_iHeight;
};
