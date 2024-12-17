#pragma once
#include <Windows.h>

#include <map>
#include <string>
#include <functional>

#include <EngineBase/EngineMath.h>

class UEngineWindow
{
public:
	ENGINEAPI static void EngineWindowInit(HINSTANCE _Instance);
	ENGINEAPI static void CreateWindowClass(const WNDCLASSEXA& _Class);
	ENGINEAPI static int WindowMessageLoop(std::function<void()> _StartFunction, std::function<void()> _FrameFunction, std::function<void()> _EndFunction = nullptr);

	ENGINEAPI UEngineWindow();
	ENGINEAPI ~UEngineWindow();

	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	ENGINEAPI void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");
	ENGINEAPI void Open(std::string_view _TitleName = "Window");

	inline FVector GetWindowSize() const
	{
		return WindowSize;
	}

	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	ENGINEAPI void SetWindowPosAndScale(FVector _Pos, FVector _Scale);

	FVector GetMousePos();

	void ApplicationOff()
	{
		LoopActive = false;
	}

protected:

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static HINSTANCE hInstance;
	static bool LoopActive;
	static std::map<std::string, WNDCLASSEXA> WindowClasss;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
};

