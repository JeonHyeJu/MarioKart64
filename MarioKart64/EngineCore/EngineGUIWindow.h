#pragma once
#include <EngineBase/Object.h>

class UEngineGUIWindow : public UObject
{
public:
	ENGINEAPI UEngineGUIWindow();
	ENGINEAPI ~UEngineGUIWindow();

	UEngineGUIWindow(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow(UEngineGUIWindow&& _Other) noexcept = delete;
	UEngineGUIWindow& operator=(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow& operator=(UEngineGUIWindow&& _Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void OnGUI() = 0;

protected:

private:

};

