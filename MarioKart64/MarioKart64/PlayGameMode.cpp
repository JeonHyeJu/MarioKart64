#include "PreCompile.h"
#include "PlayGameMode.h"
#include "BaseMap.h"
#include "Player.h"
#include "Driver.h"
#include "Skybox.h"
#include "ItemBox.h"
#include "GameData.h"
#include "Balloons.h"
#include "ShrinkEffect.h"
#include "ExpandEffect.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineRenderTarget.h>
#include <EnginePlatform/EngineInput.h>
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
	TestItemBox->SetActorLocation({ -400.f, -125.f, 1000.f });
	//TestItemBox->SetActorLocation({ 0.f, TestItemBox->SIZE * .75f, 1000.f });

	// Temp
	Balloons = pLevel->SpawnActor<ABalloons>();
	Balloons->SetActorLocation(FVector{-400.f, 0.f, 500.f});

	// for test
	std::vector<SPlayerInfo> players = { SPlayerInfo{ ECharacter::MARIO, } };
	GameData::GetInstance()->SetPlayers(players);
	GameData::GetInstance()->SetCurMap(ECircuit::LUIGI_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::ROYAL_RACEWAY);
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::StartLuigiRaceway()
{
	const float SCALE = 3500.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });
	MapPtr->SetActorLocation({ 0.0f, 0.f, 0.f });
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

	Player->SetActorLocation({ -400.0f, -60.0f, 300.0f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ 50.f, 0.f, -200.f };
}

void APlayGameMode::StartRoyalRaceway()
{
	const float SCALE = 4;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });
	MapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	Player->SetActorLocation({ -50.0f, 100.0f, 700.0f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -90.f, 0.f, -80.f };
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();

	ULevel* pLevel = GetWorld();
	//Player = pLevel->GetMainPawn<ADriver>();
	Player = pLevel->GetMainPawn<APlayer>();
	
	Player->SetMap(MapPtr.get());
	Player->SetInitCameraLoc(CameraInitLoc);

	std::shared_ptr<ACameraActor> Camera = pLevel->GetMainCamera();
	Camera->GetCameraComponent()->SetZSort(1, true);

	Camera->AddActorLocation(CameraInitLoc);
	//Camera->AddActorLocation(CameraInitLoc + CameraMoveLoc * CAM_MOVE_SCALAR);
	Camera->AttachToActor(Player);

	// Temp
	if (GameData::GetInstance()->GetCurMap() == ECircuit::LUIGI_RACEWAY)
	{
		StartLuigiRaceway();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::ROYAL_RACEWAY)
	{
		StartRoyalRaceway();
	}

	// Add effect
	{
		UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
		//UEngineRenderTarget* target = GetWorld()->GetLastRenderTarget();
		//target->AddEffect<FxShrinkEffect>();
		target->AddEffect<FxExpandEffect>();

		/*std::shared_ptr<UPostEffect> Effect = lastTarget->GetPostEffect(0);
		Effect->IsActive = false;*/
	}

	State = EState::START;
}

void APlayGameMode::Tick(float _deltaTime)
{
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	if (State == EState::START)
	{
		Starting(_deltaTime);
	}
	else if (State == EState::PLAY)
	{
		Playing(_deltaTime);
	}
}

void APlayGameMode::Starting(float _deltaTime)
{
	Balloons->AddActorLocation({ 0.f, 20.f * _deltaTime, 0.f });

	if (Balloons->GetActorLocation().Y > 100)
	{
		Balloons->Destroy();
	}

	// Temp
	State = EState::PLAY;

	/*std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

	FVector camLoc = Camera->GetActorLocation();
	if (camLoc.Y > CameraInitLoc.Y)
	{
		Camera->AddActorLocation(-CameraMoveLoc * _deltaTime);
	}
	else
	{
		State = EState::PLAY;
	}*/
}

void APlayGameMode::Playing(float _deltaTime)
{
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

	//OutputDebugStringA(("playerLoc: " + std::to_string(normX) + ", " + std::to_string(normZ) + " -> " + std::to_string(x) + ", " + std::to_string(z) + "\n").c_str());

	GameData* pData = GameData::GetInstance();
	pData->SetMinimapLoc(0, loc);
	pData->SetPlayerRotation(0, rot);
}