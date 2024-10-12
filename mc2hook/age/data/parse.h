#pragma once
#include <mc2hook\mc2hook.h>

extern struct Vector2;
extern struct Vector3;
extern struct Vector4;

class datParserRecord
{
    // TODO
};

class datParser
{
private:
    uint8_t buf[0x5C];
private:
    datParserRecord& AddRecord(int type, LPCSTR name, void* dataPtr, int count) 
    {
        // last parameter is the 'callback' which is never actually used
        return hook::Thunk<0x5E2980>::Call<datParserRecord&>(this, type, name, dataPtr, count, nullptr);
    };

    inline datParserRecord& AddRecord(int type, LPCSTR name, void* dataPtr) 
    {
        return AddRecord(type, name, dataPtr, 1);
    };
public:
    datParser(LPCSTR name)
    {
        hook::Thunk<0x5E28F0>::Call<void>(this, name);
    }

    virtual ~datParser()
    {
        hook::Thunk<0x5E2D60>::Call<void>(this);
    }

    bool Load(LPCSTR dir, LPCSTR file, LPCSTR extension)
    {
        return hook::Thunk<0x5E2E50>::Call<bool>(this, dir, file, extension);
    }

    datParser* AddParser(LPCSTR name) 
    {
        // 'callback' unused (see AddRecord above)
        return hook::Thunk<0x5E2D80>::Call<datParser*>(this, name, nullptr);
    };

    /*
        Support for adding arrays of values
    */
    void AddValue(LPCSTR name, bool* values, int count) { AddRecord(1, name, values, count); };
    void AddValue(LPCSTR name, char* values, int count) { AddRecord(2, name, values, count); };
    void AddValue(LPCSTR name, short* values, int count) { AddRecord(3, name, values, count); };
    void AddValue(LPCSTR name, int* values, int count) { AddRecord(4, name, values, count); };
    void AddValue(LPCSTR name, float* values, int count) { AddRecord(5, name, values, count); };
    void AddValue(LPCSTR name, Vector2* values, int count) { AddRecord(6, name, values, count); };
    void AddValue(LPCSTR name, Vector3* values, int count) { AddRecord(7, name, values, count); };
    void AddValue(LPCSTR name, Vector4* values, int count) { AddRecord(8, name, values, count); };

    /*
        Methods for adding a single value instead of an array
    */

    void AddValue(LPCSTR name, bool* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, char* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, short* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, int* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, float* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, Vector2* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, Vector3* value) { AddValue(name, value, 1); };
    void AddValue(LPCSTR name, Vector4* value) { AddValue(name, value, 1); };
};