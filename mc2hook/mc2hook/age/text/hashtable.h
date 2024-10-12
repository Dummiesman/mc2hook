#pragma once
#include <mc2hook\mc2hook.h>

extern struct txtStringData;

struct txtHashEntry
{
    int hash;
    txtStringData* data;
    txtHashEntry* next;
};

struct txtHashPosition
{
    txtHashEntry* entry;
    int index;

    txtStringData* GetData()
    {
        return (entry) ? entry->data : nullptr;
    }

    txtHashPosition()
    {
        entry = nullptr;
        index = -1;
    }
};

class txtHashTable
{
private:
    int32_t numSlots;
    txtHashEntry** entries;
    int32_t numEntries;
public:
    bool GetFirstEntry(txtHashPosition& position)
    {
        for (int i = 0; i < numSlots; i++)
        {
            if (entries[i])
            {
                position.entry = entries[i];
                position.index = i;
                return true;
            }
        }
        return false;
    }

    bool GetNextEntry(txtHashPosition& position)
    {
        if (position.entry && position.entry->next)
        {
            position.entry = position.entry->next;
            return true;
        }

        for (int i = position.index + 1; i < numSlots; i++)
        {
            if (entries[i])
            {
                position.entry = entries[i];
                position.index = i;
                return true;
            }
        }
        return false;
    }

    void ComputePrime()
    {
        hook::Thunk<0x611F10>::Call<void>(this);
    }

    void Insert(unsigned int hash, txtStringData* data)
    {
        hook::Thunk<0x5E2790>::Call<void>(this, hash, data);
    }
};