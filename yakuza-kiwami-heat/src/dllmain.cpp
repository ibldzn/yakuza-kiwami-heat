#include <iostream>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include "hooks/hooks.h"
#include "mem/mem.h"

DWORD WINAPI entry(LPVOID lp_param)
{
	try
	{
		mem::init();
		hooks::init();
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(NULL, ex.what(), "ERROR", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(lp_param), EXIT_FAILURE);
	}

	while (!(GetAsyncKeyState(VK_END) & 0x8000))
		Sleep(50);

	FreeLibraryAndExitThread(static_cast<HMODULE>(lp_param), EXIT_SUCCESS);
}

BOOL detach()
{
	mem::destroy();

	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE h_inst, DWORD dw_reason, LPVOID lp_reserved)
{
	if (dw_reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(static_cast<HMODULE>(h_inst));
		if (HANDLE thread = CreateThread(nullptr, NULL, entry, nullptr, NULL, nullptr))
			CloseHandle(thread);
	}
	else if (dw_reason == DLL_PROCESS_DETACH)
	{
		return detach();
	}

	return TRUE;
}