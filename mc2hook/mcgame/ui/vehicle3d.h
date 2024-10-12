#pragma once
#include <mc2hook/mc2hook.h>
#include "iconbase.h"
#include <age/data/parse.h>


// TODO: inheritance: public mcUiIconSlot
class mcUiVehicle3DSlot : public mcUiIconSlot
{
public:
    int32_t displayTypeIdx;
    LPCSTR name;
    float viewerHeight;
    float lookAtHeight;
    float viewerDistance;
public:
    mcUiVehicle3DSlot()
    {
        this->displayTypeIdx = 0;
        this->name = nullptr;
        this->viewerHeight = 2.0f;
        this->viewerDistance = 5.0f;
        this->lookAtHeight = 0.0f;
    }

    virtual ~mcUiVehicle3DSlot()
    {

    }

    virtual void FileIO(datParser& parser, LPCSTR prefix)
    {
        hook::Thunk<0x457BC0>::Call<void>(this, &parser, prefix);
    }
};

class mcUiVehicle3D : public mcUiIconBase
{
private:
    uint8_t buf[0x6E0];
protected:
    static hook::Field<0x730, int32_t> _vehicleCount;
    static hook::Field<0x230, mcUiVehicle3DSlot> _vehicleInfos; //[40]
public:
    void AddVehicle(int32_t unk, LPCSTR name, int32_t displayTypeIdx, int32_t index)
    {
        auto& slot = _vehicleInfos.ptr(this)[_vehicleCount.get(this)];
        slot.dword_04 = unk;
        slot.index = index;
        slot.displayTypeIdx = displayTypeIdx;
        slot.name = name;
        _vehicleCount.set(this, _vehicleCount.get(this) + 1);
    }
};

ASSERT_SIZEOF(mcUiVehicle3D, 0x75C);

// this is the one actually used in the menu
class mcUiVehicle3D_derived : public mcUiVehicle3D
{
private:
    uint8_t buf[0x0C];
public:
};
ASSERT_SIZEOF(mcUiVehicle3D_derived, 0x768);

// Now for our custom one (sort of, we'll be hooking into existing methods and changing offsets for now...)
class mcUiVehicle3D_derived_new : public mcUiVehicle3D_derived
{
private:
    mcUiVehicle3DSlot mNewSlots[512];
public:
    void AddVehicle(int32_t unk, LPCSTR name, int32_t displayTypeIdx, int32_t index)
    {
        auto& slot = mNewSlots[_vehicleCount.get(this)];
        slot.dword_04 = unk;
        slot.index = index;
        slot.displayTypeIdx = displayTypeIdx;
        slot.name = name;
        _vehicleCount.set(this, _vehicleCount.get(this) + 1);
    }

    mcUiVehicle3DSlot& GetVehicleSlot(int index)
    {
        return mNewSlots[index];
    }

    void InitNewSlots()
    {
        for (int i = 0; i < 512; i++)
        {
            (&mNewSlots[i])->mcUiVehicle3DSlot::mcUiVehicle3DSlot();
        }
        for (int i = 32; i < 512; i++)
        {
            // Setup custom vehicle cameras (TODO: load from a file?)
            auto& slot = GetVehicleSlot(i);
            slot.viewerHeight = 1.609f;
            slot.lookAtHeight = 0.02999f;
            slot.viewerDistance = 5.89996f;
        }
    }

    void DumpSlotInfo()
    {
        Warningf("== mcUiVehicle3D_derived_new slot dump ==");
        Warningf("Occuplied slot count: %i", _vehicleCount.get(this));
        for (int i = 0; i < _vehicleCount.get(this); i++)
        {
            auto& slot = GetVehicleSlot(i);
            Warningf("[Slot %i]", i);
            Warningf("DisplayTypeIdx %i", slot.displayTypeIdx);
            Warningf("VehicleIndex %i", slot.index);
            Warningf("Name %i", slot.name);
        }
    }
};