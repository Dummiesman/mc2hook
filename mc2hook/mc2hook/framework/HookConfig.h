#pragma once
#include <thirdparty/inicpp.h>

class HookConfig
{
private:
    static ini::IniFile ini;
private:
    template<typename T>
    static T Get(const char* sectionName, const char* keyName, T defaultValue);
public:
    static bool GetBool(const char* sectionName, const char* keyName, bool defaultValue);
    static const char* GetString(const char* sectionName, const char* keyName, const char* defaultValue);
    static int GetInt(const char* sectionName, const char* keyName, int defaultValue);
    static float GetFloat(const char* sectionName, const char* keyName, float defaultValue);

    static void Init();
};

