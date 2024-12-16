#pragma once
#include "EngineDefine.h"
#include <Windows.h>
#include <assert.h>


#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "??? ??", MB_OK); assert(false);

namespace UEngineDebug
{
	ENGINEAPI void LeakCheck();

	ENGINEAPI void OutPutString(const std::string& Text);
}

