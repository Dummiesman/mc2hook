#pragma once
#include <mc2hook\mc2hook.h>
#include <age\string\unicode.h>
#include <age\memory\age_alloc_baseclass.h>
#include <age/string/string.h>
#include "fonttexture.h"


struct txtStringData : public AGEAllocatedClass
{
    int hash;
    txtFontTexture* fontTexture;
    LPWSTR text;
    float scale[2];
    signed char offsetX;
    signed char offsetY;
    uint16_t flags;

    txtStringData() : hash(0), fontTexture(0), text(NULL), scale{1.0f, 1.0f}, offsetX(0), offsetY(0), flags(0)
    {

    }

    txtStringData(txtStringData& copyfrom)
    {
        hash = copyfrom.hash;

        // if texture is unloaded, type is string
        if ((intptr_t)copyfrom.fontTexture & 0x01)
        {
            auto dupAddr = StringDuplicate((char*)((intptr_t)fontTexture & 0xFFFFFFFE));
            this->fontTexture = (txtFontTexture*)(dupAddr + 0x01);
        }
        else
        {
            this->fontTexture = copyfrom.fontTexture;
            if(this->fontTexture) this->fontTexture->IncRef();
        }
        
        fontTexture = 0;
        text = WStringDuplicate(copyfrom.text);
        scale[0] = copyfrom.scale[0];
        scale[1] = copyfrom.scale[1];
        offsetX = copyfrom.offsetX;
        offsetY = copyfrom.offsetY;
        flags = copyfrom.flags;
    }

    ~txtStringData()
    {
        hook::Thunk<0x5E1370>::Call<void>(this);
    }
};