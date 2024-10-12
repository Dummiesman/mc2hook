#include "CustomVehicleHandler.h"
#include <age\data\args.h>
#include <age\string\string.h>
#include <age\text\stringtable.h>
#include <age\data\asset.h>
#include <mcgame/ui/asyncio.h>
#include <mcgame/ui/vehicle3d.h>
#include <mcgame/ui/mainmenu.h>
#include <mcgame/ui/pausemenu.h>

//#include <string.h>

// DEFINES
const int NUM_NEW_VEHICLE_SLOTS = 256;
const int NUM_DYN_VEHICLES_LIMIT = 232; // Gives 200 custom slots. Anything over 255 is not possible.

// Original vehicles data
static int ORIG_VEHICLE_COUNT = 32;
static char* ORIG_VEHICLE_BASENAMES[] = {"vp_escort", "vp_civica", "vp_jetta", "vp_integraa", "vp_gto", "vp_s2000", "vp_cbr929a", "vp_civicb",
                                    "vp_rx7", "vp_viper", "vp_puma", "vp_cosworth", "vp_m5", "vp_elise", "vp_golfa", "vp_ducati", "vp_clio",
                                    "vp_vanquish", "vp_911", "vp_lexus", "vp_nissanz", "vp_mr2", "vp_supraa", "vp_lancer", "vp_yamaha", "vp_suprab",
                                    "vp_skylinea", "vp_saleen", "vp_slf450x", "vp_cop_l", "vp_pariscop", "vp_tokyocop" };

hook::Type<char* [32]> MC2_Vehicle_Basenames = 0x6631F0; // Original vehicle name memory

// New vehicle name storage
static char* VEHICLE_BASENAMES_DYN[NUM_NEW_VEHICLE_SLOTS];
static int NUM_DYN_VEHICLES = 0;
static int NumCustomVehicles = 0;

hook::Type<mcPauseMenu*> PAUSEMENU = 0x697B90;

bool CustomVehicleHandler::StringLoadHook(LPCSTR filename, int language, int flagsMask, bool readIdentifier)
{
    auto table = reinterpret_cast<txtStringTable*>(this);
    bool returnValue = table->Load(filename, (txtLanguage::eLanguage)language, flagsMask, readIdentifier);
    
    // Load vehicle tables if they exist
    for (int i = 0; i < NUM_DYN_VEHICLES; i++)
    {
        if (ASSETP->Exists("fonts", VEHICLE_BASENAMES_DYN[i], "strtbl"))
        {
            Printf("(Custom Vehicles) Loading custom string table: %s", VEHICLE_BASENAMES_DYN[i]);
            auto table_sup = txtStringTable();
            if (table_sup.Load(VEHICLE_BASENAMES_DYN[i], (txtLanguage::eLanguage)language, flagsMask, readIdentifier))
            {
                table->Merge(table_sup);
            }
            table_sup.Kill();
        }
        else
        {
            Warningf("(Custom Vehicles) No custom string table for vehicle: %s", VEHICLE_BASENAMES_DYN[i]);
        }
    }

    return returnValue;
}

void CustomVehicleHandler::asyncio_LoadVehicleResources()
{
    reinterpret_cast<mcUiAsyncIo*>(this)->LoadVehicleResources();
}

void CustomVehicleHandler::asyncio_DestructHook()
{
    reinterpret_cast<mcUiAsyncIo*>(this)->CleanUpVehicleResources();
    hook::Thunk<0x456E30>::Call<void>(this); // call original
}

void CustomVehicleHandler::carststs_AddVehHook(LPCSTR basename, int index)
{
    // mcFeCarStats::count
    int count = *getPtr<int32_t>(this, 0x4a4);
    if (count >= 32) return;  // Don't add the extra vehicles for now
    hook::Thunk<0x4484C0>::Call<void>(this, basename, index);
}

void* CustomVehicleHandler::carstats_GetEquipmentIcon()
{
    // unknown effect
    bool char_60 = *getPtr<bool>(this, 0x60);
    if (char_60) return nullptr;

    // mcFeCarStats::selected_vehicle_index_a
    int selected_vehicle_index = *getPtr<int32_t>(this, 0x64);
    if (selected_vehicle_index >= 32) return nullptr;

    return hook::Thunk<0x448670>::Call<void*>(this); // call mcFeCarStats::GetEquipmentIcon
}

void CustomVehicleHandler::carstats_DrawHook()
{
    // Prevent drawing when either blend index is >= 32
    int selected_vehicle_index_a = *getPtr<int32_t>(this, 0x64); // mcFeCarStats::selected_vehicle_index_a
    int selected_vehicle_index_b = *getPtr<int32_t>(this, 0x68); // mcFeCarStats::selected_vehicle_index_b
    if (selected_vehicle_index_a >= 32 || selected_vehicle_index_b >= 32)
        return;

    hook::Thunk<0x448FA0>::Call<void>(this); // call mcFeCarStats::Draw
}

static void EnumCustomVehicles(LPCSTR name, bool isDir, void* userdata)
{
    // Find .vehModel files and use that as an indicator that a custom vehicle exists
    if (NUM_DYN_VEHICLES >= NUM_DYN_VEHICLES_LIMIT)
        return;
    
    char vehBaseName[128];
    strcpy_s(vehBaseName, name);
    auto dotPos = strrchr(vehBaseName, '.');
    
    if (dotPos)
    {
        if (!strcmpi(dotPos, ".vehmodel"))
        {
            *dotPos = 0x00;
            bool is_default = false;
            for (int i = 0; i < ORIG_VEHICLE_COUNT; i++)
            {
                if (!strcmpi(ORIG_VEHICLE_BASENAMES[i], vehBaseName))
                {
                    is_default = true;
                    break;
                }
            }

            if (!is_default)
            {
                VEHICLE_BASENAMES_DYN[NUM_DYN_VEHICLES++] = strdup(vehBaseName);
                NumCustomVehicles++;
            }
        }
    }
}

void CustomVehicleHandler::vehicle3d_AddHook(int unk, LPCSTR name, int displayTypeIdx, int index)
{
    reinterpret_cast<mcUiVehicle3D_derived_new*>(this)->AddVehicle(unk, name, displayTypeIdx, index);
}

void CustomVehicleHandler::vehicle3d_ConstructHook(char** a2, int a3)
{
    reinterpret_cast<mcUiVehicle3D_derived_new*>(this)->InitNewSlots();
    hook::Thunk<0x426480>::Call<void>(this, a2, a3); // Call original constructor
}

void* CustomVehicleHandler::pausemenu_ConstructHook()
{
    auto obj = hook::Thunk<0x47DFB0>::Call<void*>(this);
    reinterpret_cast<mcPauseMenu_new*>(this)->InitNewCarpal();
    return obj;
}

int CustomVehicleHandler::netmanager_GetVarHook()
{
    // When sending the player config, indicate all custom vehicles as Cocotte
    // Prevents mc2hook users from accidentally (or maliciously) crashing people with less mods / unmodded games
    int value = hook::Thunk<0x55E930>::Call<int>(this);
    Warningf("Vehicle var get in net manager: %i, clamping to %i", value, (value >= 32) ? 0 : value);
    return (value >= 32) ? 0 : value;
}

void CustomVehicleHandler::Install()
{
    // mcUiAsyncIo related
   InstallCallback("CustomVehicleHandler (1)", "Custom vehicle test: Override IsVehicleLoaded",
       &mcUiAsyncIo::IsVehicleLoaded, {
           cb::call(0x457458),
       }
   );

   InstallCallback("CustomVehicleHandler (2)", "Custom vehicle test: Override GetVehicle",
       &mcUiAsyncIo::GetVehicle, {
           cb::call(0x458AB7),
           cb::call(0x45762F),
       }
   );

   InstallCallback("CustomVehicleHandler (3)", "Custom vehicle test: Override LoadVehicleResources",
       &mcUiAsyncIo::LoadVehicleResources, {
           cb::call(0x4571AD),
       }
   );

   InstallCallback("CustomVehicleHandler (4)", "Custom vehicle test: Override vehicle color count get",
       &mcUiAsyncIo::GetVehicleColorCount, {
           cb::call(0x457497),
       }
   );

   InstallCallback("CustomVehicleHandler (5)", "Custom vehicle test: Destruct hook for cleanup",
       &asyncio_DestructHook, {
           cb::call(0x457243),
       }
   );

   InstallCallback("CustomVehicleHandler (6)", "Custom vehicle test: Hook stringtable load",
       &StringLoadHook, {
           cb::call(0x409B59), // frontend
           cb::call(0x409D5B), // race editor
           cb::call(0x40A0E9), // Gameplay
       }
   );

   // mcFeCarStats related
   InstallVTableHook("CustomVehicleHandler (7)", &carstats_DrawHook, {0x6392C0});

   InstallCallback("CustomVehicleHandler (8)", "Custom vehicle test: Prevent overflowing mcFeCarStats (TODO: rewrite mcFeCarStats too with more slots)",
       &carststs_AddVehHook, {
           cb::call(0x42A684),
       }
   );

   InstallCallback("CustomVehicleHandler (9)", "Custom vehicle test: Override equipment icon get  (TODO: rewrite mcFeCarStats too with more slots)",
       &carstats_GetEquipmentIcon, {
           cb::call(0x43C0FF),
       }
   );

   // mcUiVehicle3D related
   InstallCallback("CustomVehicleHandler (10)", "Custom vehicle test: Hook vehicle3d construct",
       &vehicle3d_ConstructHook, {
           cb::call(0x416814),
           cb::call(0x42EBB0),
           cb::call(0x4347B6),
       }
   );

   InstallCallback("CustomVehicleHandler (11)", "Custom vehicle test: Hook vehicle3d add",
       &vehicle3d_AddHook, {
           cb::call(0x42A67A), // Central
           cb::call(0x417585), // Race editor
       }
   );

   mem::write(0x4167FA + 1, sizeof(mcUiVehicle3D_derived_new));
   mem::write(0x42EB94 + 1, sizeof(mcUiVehicle3D_derived_new));
   mem::write(0x43479A + 1, sizeof(mcUiVehicle3D_derived_new));

   // All these are the offset to the mcUiVehicle3D_derived_new slots, which start at 0x768
   // Replaces all access from classes we have not rewritten yet
   mem::write(0x45865A + 3, 0x768 + 0xC);
   mem::write(0x458668 + 3, 0x768); 
   mem::write(0x4578B4 + 2, 0x768); 

   mem::write(0x459D2D + 3, 0x768); 
   mem::write(0x459D34 + 2, 0x768); 

   mem::write(0x4594D0 + 3, 0x768); 
   mem::write(0x4594D8 + 2, 0x768); 
   mem::write(0x459628 + 1, 0x768); 

   mem::write(0x459552 + 3, 0x768 + 0xC); 
   mem::write(0x459577 + 3, 0x768 + 0xC); 

   mem::write(0x45744A + 3, 0x768 + 0x08);

   mem::write(0x4575CB + 3, 0x768 + 0xC); 
   mem::write(0x4575E2 + 3, 0x768 + 0xC); 

   mem::write(0x45760D + 3, 0x768); 

   mem::write(0x4576C7 + 3, 0x768 + 0x08); 
   
   mem::write(0x457489 + 3, 0x768 + 0x08); 

   mem::write(0x4573BE + 3, 0x768 + 0xC);
   mem::write(0x4573EA + 3, 0x768 + 0xC);

   // mcFeMainMenu related
   mem::write(0x420185 + 1, sizeof(mcFeMainMenu_new));

   InstallCallback("CustomVehicleHandler (12)", "Custom vehicle test: hook ResetColorSelections",
       &mcFeMainMenu_new::ResetColorSelections, {
           cb::call(0x4279A7),
           cb::call(0x42EF3C)
       }
   );

   // All these are the offset to the mcFeMainMenu_new color slots, which start at 0x1c4 (colors1) and 0x9c4 (colors2)
   // Replaces access from classes we have not rewritten yet
   mem::write(0x42717F + 3, 0x1c4); 
   mem::write(0x42718F + 3, 0x1c4);
   mem::write(0x427166 + 3, 0x9c4);
   mem::write(0x427176 + 3, 0x9c4);

   mem::write(0x42B767 + 3, 0x1c4);
   mem::write(0x42B77A + 3, 0x1c4);
   mem::write(0x42B794 + 3, 0x1c4);
   mem::write(0x42B731 + 3, 0x9c4);
   mem::write(0x42B744 + 3, 0x9c4);
   mem::write(0x42B75E + 3, 0x9c4);

   // mcPauseMenu related
   InstallCallback("CustomVehicleHandler (13)", "Custom vehicle test: hook pause menu construct",
       &pausemenu_ConstructHook, {
           cb::call(0x47E16D),
       }
   );

   mem::write(0x47E15A + 1, sizeof(mcPauseMenu_new));

   // Network related
   InstallCallback("CustomVehicleHandler (14)", "Custom vehicle test: Hook vehicle var get",
   &netmanager_GetVarHook, {
       cb::call(0x4087D9), 
       cb::call(0x436719),
       cb::call(0x437005),
       cb::call(0x43EEEA)
   }
   );

   // new asyncio size
   mem::write(0x41630F + 1, sizeof(mcUiAsyncIo));
   mem::write(0x41FECA + 1, sizeof(mcUiAsyncIo));
   
   /* Init custom vehicle stuff */
   // Copy over base vehicles  
   for (int i = 0; i < ORIG_VEHICLE_COUNT; i++)
   {
       VEHICLE_BASENAMES_DYN[i] = ORIG_VEHICLE_BASENAMES[NUM_DYN_VEHICLES++];
   }

   // Get custom vehicles
   ASSETP->EnumFiles("tune/vehicle", &EnumCustomVehicles, nullptr, false);

   // FE models
   mem::write(0x455928 + 3, &VEHICLE_BASENAMES_DYN);

   // This controls CENTRAL vehicle menu names, and menu icons
   mem::write(0x42A620 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x42A68A + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // This controls RACE EDITOR vehicle menu names, and menu icons
   mem::write(0x417510 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x417592 + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // Race editor again
   mem::write(0x416C73 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x416DBE + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // This is used in the Player info in the log 
   mem::write(0x53AEC0 + 3, &VEHICLE_BASENAMES_DYN);

   // AI opponent basenames
   mem::write(0x4AA127 + 3, &VEHICLE_BASENAMES_DYN);

   // Player basenames
   mem::write(0x467265 + 3, &VEHICLE_BASENAMES_DYN);

   // Vehicle selection streams
   mem::write(0x4576CE + 3, &VEHICLE_BASENAMES_DYN);

   // Lookup car
   mem::write(0x53D010 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x53D026 + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // Pause menu change vehicle
   mem::write(0x47FFE3 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x480199 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x47DE23 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x47DE32 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x47D368 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x47D3AE + 3, &VEHICLE_BASENAMES_DYN);

   mem::write(0x540700 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x540716 + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // Pause menu wrap around limits
   mem::write(0x47D486 + 3, NUM_DYN_VEHICLES - 1);
   mem::write(0x47D4C7 + 3, NUM_DYN_VEHICLES - 1);
   mem::write(0x47D4B9 + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES));

   // Opponent car type save
   mem::write(0x413436 + 3, &VEHICLE_BASENAMES_DYN);

   // Network vehicle slots
   mem::write(0x422023 + 3, &VEHICLE_BASENAMES_DYN);
   mem::write(0x42201E + 2, static_cast<uint8_t>(NUM_DYN_VEHICLES)); // Index to stop drawing names at
   mem::write(0x4252F5 + 1, static_cast<uint8_t>(NUM_DYN_VEHICLES)); // Waiting for player draw
   

   // Setup asyncio
   mcUiAsyncIo::s_VehicleCount = NUM_DYN_VEHICLES;
}
