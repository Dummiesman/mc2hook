#include "HookConfig.h"

ini::IniFile HookConfig::ini;

template<typename T>
T HookConfig::Get(const char* sectionName, const char* keyName, T defaultValue) 
{
    auto section = ini.find(sectionName);
    if (section == ini.end()) 
        return defaultValue;

    auto value = section->second.find(keyName);
    if (value == section->second.end()) 
        return defaultValue;

    return value->second.as<T>();
}

bool HookConfig::GetBool(const char* sectionName, const char* keyName, bool defaultValue)
{
    // mm2hook style
    auto strVal = GetString(sectionName, keyName, nullptr);
    if (strVal == nullptr)
    {
        return defaultValue;
    }
    return strcmp(strVal, "1") == 0 || stricmp(strVal, "YEP") == 0 || stricmp(strVal, "TRUE") == 0;
}

const char* HookConfig::GetString(const char* sectionName, const char* keyName, const char* defaultValue)
{
    return Get<const char*>(sectionName, keyName, defaultValue);
}

int HookConfig::GetInt(const char* sectionName, const char* keyName, int defaultValue)
{
    return Get<int>(sectionName, keyName, defaultValue);
}

float HookConfig::GetFloat(const char* sectionName, const char* keyName, float defaultValue)
{
    return Get<float>(sectionName, keyName, defaultValue);
}

void HookConfig::Init()
{
    HookConfig::ini = ini::IniFile();
    ini.load("mc2hook.ini");
}
