#pragma once
#include <mc2hook\mc2hook.h>
#include <age\data\refcount.h>

class mcUiFlowControl
{
public:

    class SelectionOption : public datRefCount
    {
    private:
        char owns_text;
        wchar_t* text;
        wchar_t* help_message;
        char has_id1_maybe;
        int id1;
        int dword_20;
        int id2;
        int dword_28;
        int dword_2c;
        int dword_30;
    public:
        void SetText(LPWSTR text, bool clone)
        {
            hook::Thunk<0x44B900>::Call<void>(this, text, clone);
        }

        void SetTextFromStringtable(LPCSTR key)
        {
            hook::Thunk<0x44BBF0>::Call<void>(this, key);
        }

        void SetHelpTextFromStringtable(LPCSTR key)
        {
            hook::Thunk<0x44B950>::Call<void>(this, key);
        }

        void SetIcon(LPCSTR textureName)
        {
            hook::Thunk<0x44B990>::Call<void>(this, textureName);
        }
    };

    class SelectionGroup : public datRefCount
    {
    private:
        int dword_08;
        int dword_0c;
        int dword_10;
        int dword_14;
        int dword_18;
        int dword_1c;
        int dword_20;
        int dword_24;
        int dword_28;
        int dword_2c;
        int dword_30;
        int dword_34;
        int dword_38;
        int dword_3c;
        int dword_40;
        int dword_44;
        int dword_48;
        LPWSTR help_message;
        int dword_50;
    public:
        SelectionOption* AddOption(LPCSTR languageKey, LPCSTR icon, int index, void* unknown, int index2, bool loadHelpText)
        {
            return hook::Thunk<0x44D1C0>::Call<SelectionOption*>(this, languageKey, icon, index, unknown, index2, loadHelpText);
        }
    };
};