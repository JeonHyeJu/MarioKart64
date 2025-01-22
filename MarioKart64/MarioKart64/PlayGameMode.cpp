#include "PreCompile.h"
#include "PlayGameMode.h"
#include "BaseMap.h"
#include "Player.h"
#include "Driver.h"
#include "Skybox.h"
#include "ItemBox.h"
#include "GameData.h"
#include <EngineCore/CameraActor.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCamera.h>
#include <EngineBase/EngineMath.h>

APlayGameMode::APlayGameMode()
{
	ULevel* pLevel = GetWorld();
	pLevel->CreateCollisionProfile("ItemBox");
	pLevel->CreateCollisionProfile("Player");
	pLevel->CreateCollisionProfile("Item");
	pLevel->LinkCollisionProfile("Player", "ItemBox");
	pLevel->LinkCollisionProfile("Player", "Item");

	Skybox = pLevel->SpawnActor<ASkybox>();
	MapPtr = pLevel->SpawnActor<ABaseMap>();
	
	TestItemBox = pLevel->SpawnActor<AItemBox>();
	TestItemBox->SetActorLocation({ 0.f, TestItemBox->SIZE * .75f, 1000.f });
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::StartLuigiRaceway()
{
	MapPtr->Init(ECircuit::LUIGI_RACEWAY);
	MapPtr->SetActorRelativeScale3D({ 3500.f, 3500.f, 3500.f });
	MapPtr->SetActorLocation({ 0.0f, 0.f, 0.f });
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

	Player->SetActorLocation({ -400.0f, -60.0f, 300.0f });
}

void APlayGameMode::StartRoyalRaceway()
{
	MapPtr->Init(ECircuit::ROYAL_RACEWAY);
	MapPtr->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
	MapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	Player->SetActorLocation({ -50.0f, 100.0f, 700.0f });
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();

	ULevel* pLevel = GetWorld();
	//Player = pLevel->GetMainPawn<ADriver>();
	Player = pLevel->GetMainPawn<APlayer>();
	
	FVector initCamLoc = { 0.f, 100.f, -300.f };
	Player->SetMap(MapPtr.get());
	Player->SetInitCameraLoc(initCamLoc);

	std::shared_ptr<ACameraActor> Camera = pLevel->GetMainCamera();
	//Camera->GetCameraComponent()->SetZSort(1, true);

	Camera->SetActorLocation(initCamLoc);
	Camera->AttachToActor(Player);

	//StartRoyalRaceway();
	StartLuigiRaceway();
}

void APlayGameMode::Tick(float _deltaTime)
{
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	// temp. for test
	if (UEngineInput::IsPress('R'))
	{
		MapPtr->AddActorRotation({ -1.f, 0.f, 0.f });
	}
	else if (UEngineInput::IsPress('T'))
	{
		MapPtr->AddActorRotation({ 1.f, 0.f, 0.f });
	}

	FVector loc = Player->GetActorLocation();
	FVector rot = Player->GetTransform().Rotation;
	GameData::GetInstance()->SetMinimapLoc(0, loc);
	GameData::GetInstance()->SetPlayerRotation(0, rot);
	//CheckCollision(_deltaTime);
}

