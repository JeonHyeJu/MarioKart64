#pragma once
#include <EngineBase/EngineDefine.h>

class UEngineWindow
{
public:
	ENGINEAPI UEngineWindow();
	ENGINEAPI ~UEngineWindow();

	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

protected:

private:

};

