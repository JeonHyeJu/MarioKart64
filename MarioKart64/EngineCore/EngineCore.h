#pragma once
#include <EngineBase/EngineDefine.h>

class UEngineCore
{
public:
	ENGINEAPI UEngineCore();
	ENGINEAPI virtual ~UEngineCore() = 0;
	
	ENGINEAPI static void EngineStart(HINSTANCE _Instance);

	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

protected:

private:

};

