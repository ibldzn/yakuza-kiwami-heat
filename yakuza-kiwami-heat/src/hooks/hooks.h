#pragma once

#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#  include <Xinput.h>
#undef WIN32_LEAN_AND_MEAN

#include "../mem/mem.h"

namespace hooks
{
	void init();

	namespace heat_manager
	{
		using fn = __int64(__fastcall*)(__int64 this_ptr, float amount);
		__int64 __fastcall hook_func(__int64 this_ptr, float amount);
	}
}