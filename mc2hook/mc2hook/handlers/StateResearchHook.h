#pragma once

class StateResearchHook
{
private:
    void EnterState(int num);
    void SwitchStateMaybe(int num);
public:
    static void Install();
};