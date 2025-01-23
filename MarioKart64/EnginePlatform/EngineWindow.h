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

	ENGINEAPI inline FVector GetWindowSize() const
	{
		return WindowSize;
	}

	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	ENGINEAPI void SetWindowPosAndScale(FVector _Pos, FVector _Scale);

	ENGINEAPI FVector GetMousePos();

	ENGINEAPI static void ApplicationOff()
	{
		LoopActive = false;
	}

	ENGINEAPI static int GetWheelDir();

	ENGINEAPI static bool IsApplicationOn()
	{
		return LoopActive;
	}

	ENGINEAPI bool IsMouseScreenOut() const;

	ENGINEAPI HWND GetWindowHandle() const
	{
		return WindowHandle;
	}

	ENGINEAPI static  void SetCustomProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> _CustomProc);

	ENGINEAPI bool IsFocus()
	{
		return IsFocusValue;
	}

	
protected:

private:
	ENGINEAPI static std::function<bool(HWND, UINT, WPARAM, LPARAM)> CustomProc;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	ENGINEAPI static HINSTANCE hInstance;

	inline static bool LoopActive = true;

	ENGINEAPI static std::map<std::string, WNDCLASSEXA> WindowClasss;

	ENGINEAPI static std::map<HWND, UEngineWindow*> AllWindows;

	bool IsFocusValue = false;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
};


