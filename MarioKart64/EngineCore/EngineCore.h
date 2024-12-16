#pragma once
#include <EngineBase/EngineDefine.h>
#include <EnginePlatform/EngineWindow.h>
#include "IContentsCore.h"
#include <memory>

class UEngineCore
{
public:
	ENGINEAPI UEngineCore();
	ENGINEAPI virtual ~UEngineCore() = 0;

	ENGINEAPI static void EngineStart(HINSTANCE _Instance, std::string_view _DllName);

	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

protected:

private:
	static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;

	static void WindowInit(HINSTANCE _Instance);
	static void LoadContents(std::string_view _DllName);
};

