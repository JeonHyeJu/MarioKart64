#pragma once
#include <EngineBase/EngineDefine.h>
#include <EngineBase/EngineTimer.h>
#include <EngineBase/EngineString.h>
#include <EnginePlatform/EngineWindow.h>
#include "EngineGraphicDevice.h"
#include "IContentsCore.h"
#include <EnginePlatform/EngineWorkThreadPool.h>
#include "Level.h"
#include <memory>

class UEngineCore
{
public:

	ENGINEAPI static void EngineStart(HINSTANCE _Instance, std::string_view _DllName);

	template<typename GameModeType, typename MainPawnType, typename HUDType>
	static class std::shared_ptr<class ULevel> CreateLevel(std::string_view _Name)
	{
		std::string UpperString = UEngineString::ToUpper(_Name);

		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(UpperString);

		std::shared_ptr<GameModeType> GameMode = NewLevel->SpawnActor<GameModeType>();
		std::shared_ptr<MainPawnType> Pawn = NewLevel->SpawnActor<MainPawnType>();
		std::shared_ptr<HUDType> HUD = NewLevel->SpawnActor<HUDType>();

		NewLevel->InitLevel(GameMode.get(), Pawn.get(), HUD.get());

		return NewLevel;
	}

	ENGINEAPI static void OpenLevel(std::string_view _Name);


	ENGINEAPI static FVector GetScreenScale();

	ENGINEAPI static UEngineGraphicDevice& GetDevice();

	ENGINEAPI static UEngineWindow& GetMainWindow();

	ENGINEAPI static std::map<std::string, std::shared_ptr<class ULevel>> GetAllLevelMap();

	ENGINEAPI static class UGameInstance* GetGameInstance();

	ENGINEAPI static class UEngineWorkThreadPool& GetThreadPool();

	template<typename Type>
	static void CreateGameInstance()
	{
		SetGameInstance(std::make_shared<Type>());
	}



protected:

private:
	std::shared_ptr<class UGameInstance> GameInstance;

	UEngineWorkThreadPool ThreadPool;

	UEngineWindow MainWindow;

	UEngineGraphicDevice Device;

	HMODULE ContentsDLL;
	std::shared_ptr<IContentsCore> Core;
	UEngineInitData Data;

	UEngineTimer Timer;

	static void WindowInit(HINSTANCE _Instance);
	static void LoadContents(std::string_view _DllName);

	static void EngineFrame();
	static void EngineEnd();

	ENGINEAPI static std::shared_ptr<ULevel> NewLevelCreate(std::string_view _Name);

	std::map<std::string, std::shared_ptr<class ULevel>> LevelMap;
	std::shared_ptr<class ULevel> CurLevel;
	std::shared_ptr<class ULevel> NextLevel;

	ENGINEAPI static void SetGameInstance(std::shared_ptr<UGameInstance> _Inst);

	ENGINEAPI UEngineCore();
	ENGINEAPI virtual ~UEngineCore();
};

ENGINEAPI extern class UEngineCore* GEngine;