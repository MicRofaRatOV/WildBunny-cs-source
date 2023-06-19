#include "bhop.h"
#include <thread>
#include <iostream>


void bhop(bool& active, DWORD localPlayerSrc, DWORD flags, DWORD forceJump, bool& kill, const short frequency = 10)
{
    const auto mem = Memory("hl2.exe");
    // hl2.exe found

    DWORD client = mem.GetModuleAddress("client.dll");
    // client.dll found

    while (true)
    {
        if (kill)
            break;
        // we don't need this running a billion times per second :)
        std::this_thread::sleep_for(std::chrono::milliseconds(frequency));

        DWORD localPlayer = mem.Read<std::uintptr_t>(client + localPlayerSrc);

        if (!localPlayer)
            continue;
        if (!active)
            continue;

        // Cheat starting here

        DWORD localPlayerFlags = mem.Read<std::uintptr_t>(localPlayer + flags);

        // bhop
        if (GetAsyncKeyState(VK_SPACE))
            (localPlayerFlags & (1 << 0)) ?
            mem.Write<std::uintptr_t>(client + forceJump, 6) :
            mem.Write<std::uintptr_t>(client + forceJump, 4);
    }
}