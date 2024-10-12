#pragma once

class TestPanelHandler
{
public:
    void nullsub();
    void* GetPanelModel(int panelNum, int lod);
    void* FindBangerType(const char* type);
    static void Install();
};