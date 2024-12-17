#pragma once
#include "EngineDefine.h"
#include <Windows.h>
#include <assert.h>


#define MSGASSERT(VALUE) std::string ErrorText = VALUE; MessageBoxA(nullptr, ErrorText.c_str(), "치명적 에러", MB_OK); assert(false);

namespace UEngineDebug
{
	ENGINEAPI void LeakCheck();

	ENGINEAPI void OutPutString(const std::string& Text);
	ENGINEAPI void StartConsole();
	ENGINEAPI void EndConsole();
}

