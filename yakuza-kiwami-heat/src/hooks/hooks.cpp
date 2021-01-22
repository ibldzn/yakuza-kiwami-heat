#include "hooks.h"

static hooks::heat_manager::fn o_heat_manager = nullptr;

void hooks::init()
{
    auto hook_at = mem::follow(mem::ida_pattern_scan(NULL, "E8 ? ? ? ? 0F 2F F7 72 10"));
    if (!mem::hook(hook_at, hooks::heat_manager::hook_func, &o_heat_manager))
        throw std::runtime_error("Failed to hook heat_manager!");
}

__int64 __fastcall hooks::heat_manager::hook_func(__int64 this_ptr, float amount)
{
    static short index = []()
    {
        XINPUT_STATE dummy{};
        for (short i = 0; i < 4; ++i)
            if (XInputGetState(i, &dummy) == ERROR_SUCCESS)
                return i;

        return static_cast<short>(-1);
    }();

    if (index != -1)
    {
        XINPUT_STATE state{};
        XInputGetState(index, &state);

        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
            amount = 0.0f;
        else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
            amount = FLT_MAX;
    }

    return o_heat_manager(this_ptr, amount);
}