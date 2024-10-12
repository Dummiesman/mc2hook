#pragma once
#include <mc2hook\mc2hook.h>
#include <age\core\output.h>

class mcPauseMenu
{
protected:
    int32_t dword_04;
    int32_t selected_vehicle_a;
    int32_t selected_vehicle_b;
    int32_t last_selected_vehicle_a;
    int32_t last_selected_vehicle_b;
    int32_t selected_color_a;
    int32_t selected_color_b;
    int32_t last_selected_color_a;
    int32_t last_selected_color_b;
    int32_t dword_28;
    int32_t dword_2c;
    int32_t dword_30;
    int32_t dword_34;
    int32_t dword_38;
    int32_t dword_3c;
    int32_t dword_40;
    int32_t dword_44;
    int32_t pSndHudCounter;
    int32_t dword_4c;
    int32_t dword_50;
    int32_t dword_54;
    int32_t dword_58;
    int32_t dword_5c;
    int32_t dword_60;
    char carpal[32][4];
    int32_t some_veh_index;
public:
    virtual ~mcPauseMenu()
    {
        hook::Thunk<0x47C950>::Call<void>(this);
    }

    int GetSelectedColor(int player) const
    {
        return (player == 0) ? selected_color_a : selected_color_b;
    }

    int GetSelectedVehicle(int player) const
    {
        return (player == 0) ? selected_vehicle_a : selected_vehicle_b;
    }

    int GetPaletteIndex(int vehicle, int color) const
    {
        return carpal[vehicle][color];
    }
};
ASSERT_SIZEOF(mcPauseMenu, 0xE8);

class mcPauseMenu_new
{
private:
    int32_t dword_04;
    int32_t selected_vehicle_a;
    int32_t selected_vehicle_b;
    int32_t last_selected_vehicle_a;
    int32_t last_selected_vehicle_b;
    int32_t selected_color_a;
    int32_t selected_color_b;
    int32_t last_selected_color_a;
    int32_t last_selected_color_b;
    int32_t dword_28;
    int32_t dword_2c;
    int32_t dword_30;
    int32_t dword_34;
    int32_t dword_38;
    int32_t dword_3c;
    int32_t dword_40;
    int32_t dword_44;
    int32_t pSndHudCounter;
    int32_t dword_4c;
    int32_t dword_50;
    int32_t dword_54;
    int32_t dword_58;
    int32_t dword_5c;
    int32_t dword_60;
    char carpal_extended[512][4]; // Yes this overwrite the last 4 bytes, but as far as I can tell they go unused
public:
    // call *after* the base constructor
    void InitNewCarpal()
    {
        // setup mod slots carpals
        for (int i = 32; i < 512; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                carpal_extended[i][j] = (j + 1);
            }
        }
    }

    int GetPaletteIndex(int vehicle, int color) const
    {
        return carpal_extended[vehicle][color];
    }
};