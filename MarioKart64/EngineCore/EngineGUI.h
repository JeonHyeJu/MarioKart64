#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class UEngineGUI
{
public:
	UEngineGUI();
	~UEngineGUI();

	UEngineGUI(const UEngineGUI& _Other) = delete;
	UEngineGUI(UEngineGUI&& _Other) noexcept = delete;
	UEngineGUI& operator=(const UEngineGUI& _Other) = delete;
	UEngineGUI& operator=(UEngineGUI&& _Other) noexcept = delete;

	static void Init();
	static void Release();

	static void GUIRenderStart();

	static void GUIRenderEnd();

	ENGINEAPI static void PushGUIWindow(std::shared_ptr<class UEngineGUIWindow> _Window);

	template<typename WindowType>
	static std::shared_ptr<WindowType> CreateGUIWindow(std::string _Text)
	{
		std::shared_ptr<WindowType> Window = std::make_shared<WindowType>();
		Window->SetName(_Text);
		PushGUIWindow(Window);
		return Window;
	}

	static void GUIRender(ULevel* _Level);

protected:

private:
	static inline std::map<std::string, std::shared_ptr<class UEngineGUIWindow>> Windows;

};

