#include "hook.h"

LPCSTR hook_types[int(cb::type::_count)] = {
    "jmp",
    "call",
    "push"
};

void InstallPatch(LPCSTR description,
    std::initializer_list<unsigned char> bytes,
    std::initializer_list<unsigned int> addresses)
{
    const auto begin = bytes.begin();
    const auto size = bytes.size();

    hook_output(" - Installing patch [%08X : %08X]:", begin, size);

    if (description != NULL)
        hook_output(" - Description: %s", description);

    for (auto addr : addresses)
    {
        hook_output("   => %08X", addr);

        mem::copy(LPVOID(addr), begin, size);
    }
}

void InstallCallback(auto_ptr lpAddr, const cb::info& callback)
{
    auto addr = callback.addr;
    auto type = callback.type;

    unsigned int dwVA = lpAddr.dwHandler;
    unsigned int dwRVA = (dwVA - (addr.dwHandler + 5));

    switch (type)
    {
    case cb::type::call:
    {
        if (mem::read<byte>(addr) == 0xFF) {
            mem::write<byte, uint, byte>(addr, 0xE8, dwRVA, 0x90);
        }
        else {
            mem::write<byte, uint>(addr, 0xE8, dwRVA);
        }
    } break;

    case cb::type::jmp:
    {
        if (mem::read<byte>(addr) == 0xFF) {
            mem::write<byte, uint, byte>(addr, 0xE9, dwRVA, 0x90);
        }
        else {
            mem::write<byte, uint>(addr, 0xE9, dwRVA);
        }
    } break;

    case cb::type::push:
    {
        mem::write<byte, uint>(addr, 0x68, lpAddr);
    } break;
    }
}

void InstallCallback(LPCSTR name,
    LPCSTR description,
    auto_ptr lpAddr,
    cb::info::list callbacks)
{
    if (name != NULL)
    {
        hook_output(" - Installing callback [%08X] for '%s':", lpAddr, name);
    }
    else
    {
        hook_output(" - Installing callback [%08X]:", lpAddr);
    }

    if (description != NULL)
        hook_output(" - Description: %s", description);

    for (const auto cb : callbacks)
    {
        hook_output("   => [%s] %08X", hook_types[int(cb.type)], cb.addr);

        InstallCallback(lpAddr, cb);
    }
}

void InstallVTableHook(LPCSTR name, auto_ptr lpAddr, std::initializer_list<unsigned int> addresses) {
    hook_output(" - Installing V-Table hook [%08X]: '%s'...", lpAddr, name);

    for (auto addr : addresses)
    {
        hook_output("   => %08X", addr);
        mem::write<unsigned int>(LPVOID(addr), lpAddr);
    }
}
