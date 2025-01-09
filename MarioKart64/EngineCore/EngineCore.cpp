#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentsCore.h"
#include "EngineResources.h"
#include "EngineConstantBuffer.h"
#include "EngineGUI.h"
#include "Level.h"


UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return MainWindow;
}

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::GetAllLevelMap()
{
	return LevelMap;
}

// 리얼 본체죠?
// UEngineGraphicDevice EngienCore.dll::UEngineCore::Device;
UEngineGraphicDevice UEngineCore::Device;
UEngineWindow UEngineCore::MainWindow;
HMODULE UEngineCore::ContentsDLL = nullptr;
std::shared_ptr<IContentsCore> UEngineCore::Core;
UEngineInitData UEngineCore::Data;
UEngineTimer UEngineCore::Timer;


std::shared_ptr<class ULevel> UEngineCore::NextLevel;
std::shared_ptr<class ULevel> UEngineCore::CurLevel = nullptr;

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::LevelMap;

FVector UEngineCore::GetScreenScale()
{
	return Data.WindowSize;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
	FreeLibrary(ContentsDLL);
}

void UEngineCore::WindowInit(HINSTANCE _Instance)
{
	UEngineWindow::EngineWindowInit(_Instance);
	MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContents(std::string_view _DllName)
{
	UEngineDirectory Dir;

	Dir.MoveParentToDirectory("Build\\Bin\\x64");

#ifdef _DEBUG
	Dir.Move("Debug");
#else
	Dir.Move("Release");
#endif
	
	UEngineFile File = Dir.GetFile(_DllName);

	std::string FullPath = File.GetPathToString();

	ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == ContentsDLL)
	{
		MSGASSERT("컨텐츠 기능을 로드할수가 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = (INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine을 정의하지 않았습니다.");
		return;
	}

	Ptr(Core);

	if (nullptr == Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::EngineStart(HINSTANCE _Instance, std::string_view _DllName)
{
	UEngineDebug::LeakCheck();

	WindowInit(_Instance);

	LoadContents(_DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			Device.CreateDeviceAndContext();
			Core->EngineStart(Data);
			MainWindow.SetWindowPosAndScale(Data.WindowPos, Data.WindowSize);
			Device.CreateBackBuffer(MainWindow);		
			UEngineGUI::Init();
		},
		[]()
		{
			EngineFrame();
		},
		[]()
		{
			EngineEnd();
		});
}

// To prevent recursive importing
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	if (true == LevelMap.contains(UpperName))
	{
		MSGASSERT("같은 이름의 레벨을 또 만들수는 없습니다." + std::string(UpperName));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(UpperName);

	LevelMap.insert({ UpperName, Ptr});

	std::cout << "NewLevelCreate" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == LevelMap.contains(UpperString))
	{
		MSGASSERT("만들지 않은 레벨로 변경하려고 했습니다." + UpperString);
		return;
	}

	NextLevel = LevelMap[UpperString];
}

void UEngineCore::EngineFrame()
{
	if (nullptr != NextLevel)
	{
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel;

		CurLevel->LevelChangeStart();
		NextLevel = nullptr;
		Timer.TimeStart();
	}

	Timer.TimeCheck();
	float DeltaTime = Timer.GetDeltaTime();
	if (true == MainWindow.IsFocus())
	{
		UEngineInput::KeyCheck(DeltaTime);
	}
	else {
		UEngineInput::KeyReset();
	}
	
	CurLevel->Tick(DeltaTime);
	CurLevel->Render(DeltaTime);
	CurLevel->Collision(DeltaTime);
	CurLevel->Release(DeltaTime);
}

void UEngineCore::EngineEnd()
{

	UEngineGUI::Release();

	Device.Release();

	UEngineResources::Release();
	UEngineConstantBuffer::Release();

	CurLevel = nullptr;
	NextLevel = nullptr;
	LevelMap.clear();
	UEngineDebug::EndConsole();
}