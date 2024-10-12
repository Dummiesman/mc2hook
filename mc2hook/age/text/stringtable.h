#pragma once
#include <mc2hook\mc2hook.h>
#include <age\text\hashtable.h>
#include <age\text\stringdata.h>
#include <age\memory\memory.h>
#include "language.h"

class txtStringTable
{
private:
    txtHashTable* m_HashTable;
    char** m_Identifiers;
    int m_NumIdentifiers;
public:
    bool Load(LPCSTR file, txtLanguage::eLanguage language, int flagsMask = 0xFFFFFFFF, bool readIdentifiers = false)
    {
        return hook::Thunk<0x5E1500>::Call<bool>(this, file, language, flagsMask, readIdentifiers);
    }

    bool Exists(LPCSTR key)
    {
        return hook::Thunk<0x5E19D0>::Call<bool>(this, key);
    }

    LPWSTR Get(LPCSTR key)
    {
        return hook::Thunk<0x5E17F0>::Call<LPWSTR>(this, key);
    }

    // Unofficial extension method
    void Insert(unsigned int a1, txtStringData* data)
    {
        m_HashTable->Insert(a1, data);
    }
    
    // Unofficial extension method
    // !Doesn't merge identifiers!
    void Merge(txtStringTable& other)
    {
        txtHashPosition hashPos = txtHashPosition();
        if (other.m_HashTable->GetFirstEntry(hashPos))
        {
            do 
            {
                txtStringData* data = age_new txtStringData(*hashPos.GetData());
                m_HashTable->Insert(data->hash, data);
            } 
            while (other.m_HashTable->GetNextEntry(hashPos));
        }
    }

    void Kill()
    {
        hook::Thunk<0x5E1A80>::Call<void>(this);
    }
};

static _TypeProxy<txtStringTable> STRINGTABLE(0x8582C8);