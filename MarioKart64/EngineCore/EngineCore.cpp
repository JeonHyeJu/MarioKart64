#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::EngineStart(HINSTANCE _Instance)
{
	UEngineDebug::LeakCheck();

	UEngineWindow::EngineWindowInit(_Instance);

	UEngineWindow MainWindow;

	MainWindow.Open("MainWindow");

	UEngineWindow::WindowMessageLoop(
		[]()
		{
		},
		[]()
		{
		},
		[]()
		{
		});
}