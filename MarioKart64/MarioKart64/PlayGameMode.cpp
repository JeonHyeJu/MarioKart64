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
	
	// Temp
	Balloons = pLevel->SpawnActor<ABalloons>();
	Balloons->SetActorLocation(FVector{-400.f, 0.f, 500.f});

	// for test
	std::vector<SPlayerInfo> players = { SPlayerInfo{ ECharacter::MARIO, } };
	GameData::GetInstance()->SetPlayers(players);
	//GameData::GetInstance()->SetCurMap(ECircuit::LUIGI_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::KOOPA_TROOPA_BEACH);
	//GameData::GetInstance()->SetCurMap(ECircuit::MARIO_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::WARIO_STADIUM);
	//GameData::GetInstance()->SetCurMap(ECircuit::SHERBET_LAND);
	GameData::GetInstance()->SetCurMap(ECircuit::ROYAL_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::BOWSERS_CASTLE);
	//GameData::GetInstance()->SetCurMap(ECircuit::RAINBOW_ROAD);
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::StartLuigiRaceway()
{
	const float SCALE = 4000.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });
	MapPtr->SetActorLocation({ 0.0f, 0.f, 0.f });
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

	Player->SetActorLocation({ -400.0f, -60.0f, 300.0f });

	//Player->SetActorLocation({ -400.0f, -60.0f, 7000.0f });

	//Player->SetActorLocation({ -1200.f, -782.f, 2641.f });
	//Player->SetActorRotation({ 0.f, 180.f, 0.f });

	//Player->SetActorLocation({ -3800.0f, -60.0f, -6600.f });
	//Player->SetActorRotation({ 0.f, 90.f, 0.f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ 50.f, 0.f, -200.f };
}

void APlayGameMode::StartKoopaBeach()
{
	const float SCALE = 50.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	//Player->SetActorLocation({ 0.0f, 20.0f, -1000.0f });

	//Player->SetActorLocation({ -395.0f, 15.0f, 4370});

	//Player->SetActorLocation({ -7911.f, 15.0f, -2318.f });

	Player->SetActorLocation({ -6686.f, 15.0f, -12073.f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -52.f, 0.f, -100.f };
}

void APlayGameMode::StartMarioRaceway()
{
	const float SCALE = 4500.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	//Player->SetActorLocation({ 0.0f, 10.0f, 0.0f });

	//Player->SetActorLocation({ 1734.f, 10.0f, 4060.0f });

	//Player->SetActorLocation({ 10238.f, 10.0f, 318.0f });

	Player->SetActorLocation({ 8657.f, 10.0f, -4202.0f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -250.f, 0.f, -55.f };
}

void APlayGameMode::StartWarioStadium()
{
	const float SCALE = 1000.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	//Player->SetActorLocation({ 5000.0f, 10.0f, 0.0f });

	//Player->SetActorLocation({ -3405.0f, 10.0f, 660.0f });
	//Player->SetActorRotation({ 0.f, 180.f, 0.f });

	//Player->SetActorLocation({ 1677.0f, 10.0f, 7043.0f });
	//Player->SetActorRotation({ 0.f, 90.f, 0.f });

	Player->SetActorLocation({ 18.0f, 10.0f, -180.0f });
	Player->SetActorRotation({ 0.f, 180.f, 0.f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -64.f, 0.f, -120.f };
}

void APlayGameMode::StartSherbetLand()
{
	const float SCALE = 50.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	Player->SetActorLocation({ 4719.f, 150.0f, -3392.0f });

	//Player->SetActorLocation({ -7407.f, 150.0f, -2594.0f });
	//Player->SetActorRotation({ 0.f, 180.0f, 0.f });

	//Player->SetActorLocation({ -9188.f, -300.0f, -10933.0f });

	//Player->SetActorLocation({ -1849.f, 150.0f, -2965.0f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -53.f, 0.f, -58.f };
}

void APlayGameMode::StartRoyalRaceway()
{
	const float SCALE = 4.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });
	MapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	//Player->SetActorLocation({ -50.0f, 100.0f, 700.0f });

	//Player->SetActorLocation({ -1500.0f, 100.0f, 8485.0f });

	Player->SetActorLocation({ 2373.0f, 100.0f, 10796.0f });

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
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::KOOPA_TROOPA_BEACH)
	{
		StartKoopaBeach();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::MARIO_RACEWAY)
	{
		StartMarioRaceway();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::WARIO_STADIUM)
	{
		StartWarioStadium();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::SHERBET_LAND)
	{
		StartSherbetLand();
	}

	// Add effect
	{
		UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
		//UEngineRenderTarget* target = GetWorld()->GetLastRenderTarget();
		//target->AddEffect<FxShrinkEffect>();
		//target->AddEffect<FxExpandEffect>();

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