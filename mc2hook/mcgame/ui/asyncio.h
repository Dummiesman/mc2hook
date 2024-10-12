#pragma once
#include <mc2hook\mc2hook.h>
#include <age\data\refcount.h>
#include <age\memory\age_alloc_baseclass.h>
#include <age/memory/memory.h>
#include <age/core/output.h>

class mcVehicleResource : public datRefCount, public AGEAllocatedClass
{
private:
    bool loadRequested;
    bool loadFinished;
    int dword_0c;
    int dword_10;
    int dword_14;
    int dword_18;
    int dword_1c;
    int dword_20;
    int resource_name_table;
    int index;
    int mdlUi; // Seems to be some struct of some kind? From here and beyond a bit.
    int mdlWheels;
    int dword_34;
    int dword_38;
    int dword_3c;
    int dword_40;
    int dword_44;
    int dword_48;
    int dword_4c;
    int dword_50;
    int dword_54;
    int dword_58;
    int dword_5c;
    int dword_60;
    int dword_64;
    int dword_68;
    int dword_6c;
    int dword_70;
    int dword_74;
    int dword_78;
    int dword_7c;
    int dword_80;
    int dword_84;
    int dword_88;
    int dword_8c;
    int dword_90;
    int dword_94;
    int dword_98;
    int dword_9c;
    int dword_a0;
    int dword_a4;
    int dword_a8;
    int dword_ac;
    int dword_b0;
    int dword_b4;
    int dword_b8;
    int dword_bc;
    int dword_c0;
    int dword_c4;
    int dword_c8;
    int dword_cc;
    int dword_d0;
    int dword_d4;
    int dword_d8;
    int dword_dc;
    int dword_e0;
    int dword_e4;
    int dword_e8;
    int dword_ec;
    int dword_f0;
    char textureSwapNames[8][30];
    int color_count;
    int numTexturesToSwap;
public:
    mcVehicleResource(int index)
    {
        //scoped_vtable x(this); we actually want the vtable from the base for now
        //since we don't have everything fulled RE'd
        hook::Thunk<0x4551D0>::Call<void>(this, index);
    }

    void LoadModel(int a2, int a3)
    {
        hook::Thunk<0x455910>::Call<void>(this, a2, a3);
    }

    bool IsLoaded() const
    {
        return this->loadFinished;
    }

    bool HasModels() const
    {
        return mdlUi != 0 || mdlWheels != 0;
    }

    void UnkMdlFunc(int a1)
    {
        // does something to the model, idk what yet
        hook::Thunk<0x4554B0>::Call<void>(this, a1);
    }

    void* GetReturnPtr() const
    {
        // I guess this is a struct in a struct?
        return (void*) &mdlUi;
    }

    int GetColorCount() const
    {
        return this->color_count;
    }
};
ASSERT_SIZEOF(mcVehicleResource, 0x1EC);

class mcUiAsyncIo : public datRefCount
{
public:
    static int32_t s_VehicleCount;
private:
    mcVehicleResource* vehresources[32];
    int dword_88;
    int dword_8c;
    int dword_90;
    int dword_94;
    char bool_98;
    char m_asynchronous;
    char m_loadupfront;
    char bool_9b;
    int m_archive;
    char dword_a0;
    int m_pagingdestination;
    int m_datatype;
    int m_vehpageslot;
    int dword_b0;
    int dword_b4;
private:
    mcVehicleResource* m_expanded_vehresources[512]; // Freeee of the limit! Just need to rewrite any access functions...
public:
    bool IsVehicleLoaded(int index) const
    {
        //Warningf("isVehicleLoaded %i", index);
        return m_expanded_vehresources[index]->IsLoaded();
    }

    void LoadVehicleResources()
    {
        if (!m_loadupfront) Quitf("Uh oh, the mcUiAsyncIo rewrite doesn't support anything but upfront loading!");

        memset(m_expanded_vehresources, 0x00, sizeof(m_expanded_vehresources));
        for (int i = 0; i < s_VehicleCount; i++) 
        {
            auto resource = age_new mcVehicleResource(i);
            ASSIGN_REF(m_expanded_vehresources[i], resource);
        }
        for (int i = 0; i < s_VehicleCount; i++)
        {
            m_expanded_vehresources[i]->LoadModel(0, 0);
        }
    }

    void CleanUpVehicleResources()
    {
        for (int i = 0; i < s_VehicleCount; i++)
        {
            ASSIGN_REF(m_expanded_vehresources[i], nullptr);
        }
    }

    void* GetVehicle(int index, int a3)
    {
        //Warningf("GetVehicle %i", index);
        if (!m_expanded_vehresources[index])
            return false;

        m_expanded_vehresources[index]->UnkMdlFunc(a3);
        return m_expanded_vehresources[index]->GetReturnPtr(); // ?
    }

    int GetVehicleColorCount(int index)
    {
        if (!m_expanded_vehresources[index])
            return false;
        int colors = m_expanded_vehresources[index]->GetColorCount();
        return (colors == 0) ? 1 : colors;
    }
};
//ASSERT_SIZEOF(mcUiAsyncIo, 0xB8);