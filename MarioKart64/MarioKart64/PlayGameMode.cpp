#include "PreCompile.h"
#include "PlayGameMode.h"
#include "BaseMap.h"
#include "Player.h"
#include "Driver.h"
#include "Skybox.h"
#include "ItemBox.h"
#include "GameData.h"
#include "Balloons.h"
#include "Lakitu.h"
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
	Lakitu = pLevel->SpawnActor<ALakitu>();
	
	// Temp
	Balloons = pLevel->SpawnActor<ABalloons>();

	// for test
	std::vector<SPlayerInfo> players = { SPlayerInfo{ ECharacter::MARIO, } };
	GameData::GetInstance()->SetPlayers(players);
	GameData::GetInstance()->SetCurMap(ECircuit::LUIGI_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::KOOPA_TROOPA_BEACH);
	//GameData::GetInstance()->SetCurMap(ECircuit::MARIO_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::WARIO_STADIUM);
	//GameData::GetInstance()->SetCurMap(ECircuit::SHERBET_LAND);
	//GameData::GetInstance()->SetCurMap(ECircuit::ROYAL_RACEWAY);
	//GameData::GetInstance()->SetCurMap(ECircuit::BOWSERS_CASTLE);
	//GameData::GetInstance()->SetCurMap(ECircuit::RAINBOW_ROAD);

	Camera = GetWorld()->GetMainCamera();
	Camera->GetCameraComponent()->GetCameraTarget()->SetClearColor({ 0.f, 0.f, 0.f, 1.f });

	Fsm.CreateState(EState::READY, std::bind(&APlayGameMode::Readying, this, std::placeholders::_1), std::bind(&APlayGameMode::OnGetReady, this));
	Fsm.CreateState(EState::WAIT_TITLE, std::bind(&APlayGameMode::WaitingTitle, this, std::placeholders::_1));
	Fsm.CreateState(EState::COUNT, std::bind(&APlayGameMode::Counting, this, std::placeholders::_1), std::bind(&APlayGameMode::OnCount, this));
	Fsm.CreateState(EState::PLAY, std::bind(&APlayGameMode::Playing, this, std::placeholders::_1), std::bind(&APlayGameMode::OnPlay, this));
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();

	ULevel* pLevel = GetWorld();
	//Player = pLevel->GetMainPawn<ADriver>();
	Player = pLevel->GetMainPawn<APlayer>();
	
	Player->SetMap(MapPtr.get());
	//Player->SetInitCameraLoc(CameraInitLoc);

	Camera->GetCameraComponent()->SetZSort(0, true);

	// Temp
	if (GameData::GetInstance()->GetCurMap() == ECircuit::LUIGI_RACEWAY)
	{
		StartLuigiRaceway();
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
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::ROYAL_RACEWAY)
	{
		StartRoyalRaceway();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::BOWSERS_CASTLE)
	{
		StartBowserCastle();
	}
	else if (GameData::GetInstance()->GetCurMap() == ECircuit::RAINBOW_ROAD)
	{
		StartRainbowRoad();
	}

	InitEffects();
	ChangeCamIdx = 0;

	Fsm.ChangeState(EState::READY);
}

void APlayGameMode::Tick(float _deltaTime)
{
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void APlayGameMode::InitEffects()
{
	UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
	target->AddEffect<FxShrinkEffect>();
	target->AddEffect<FxExpandEffect>();

	target->GetPostEffect(0)->IsActive = false;
	//target->GetPostEffect(1)->IsActive = false;
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

	//Player->SetActorLocation({ 2373.0f, 100.0f, 10796.0f });

	Player->SetActorLocation({ -8662.0f, 0.0f, 2019.0f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -90.f, 0.f, -80.f };
}

void APlayGameMode::StartBowserCastle()
{
	const float SCALE = 4000.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	Player->SetActorLocation({ 0.f, 0.f, 0.f });

	//Player->SetActorLocation({ -82.f, 0.f, 6059.f });

	//Player->SetActorLocation({ 4909.f, 0.f, 3198.f });

	//Player->SetActorLocation({ 4838.f, 0.f, -1705.f });
	//Player->SetActorRotation({ 0.f, -90.f, 0.f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -196.f, 0.f, -200.f };
}

void APlayGameMode::StartRainbowRoad()
{
	Skybox->SetActive(false);

	const float SCALE = 100.f;
	ECircuit type = GameData::GetInstance()->GetCurMap();
	MapPtr->Init(type);
	MapPtr->SetActorRelativeScale3D({ SCALE, SCALE, SCALE });

	//Player->SetActorLocation({ 900.f, 5050.f, -1000.f });

	//Player->SetActorLocation({ 1509.f, 2435.f, 24884.f });

	//Player->SetActorLocation({ -605.f, 2155.f, 15872.f });

	//Player->SetActorLocation({ -6979.f, -80.f, 4569.f });

	//Player->SetActorLocation({ -8419.f, 1035.f, -5402.f });

	//Player->SetActorLocation({ -7871.f, 1155.f, -11415.f });

	//Player->SetActorLocation({ 7310.f, 1155.f, -10154.f });

	//Player->SetActorLocation({ -364.f, 2137.f, -13920.f });

	Player->SetActorLocation({ 2654.f, 4050.f, -12532.f });

	// Temp
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	refSize.Min *= SCALE;
	refSize.Max *= SCALE;
	GameData::GetInstance()->MapSizeInfo = SRenderInfo::MapInfos.find(type)->second.MapSizeInfo;
	GameData::GetInstance()->MapSizeInfo.InitLoc = FVector{ -83.f, 0.f, -140.f };
}

/* Fsm start function */
void APlayGameMode::OnGetReady()
{
	Camera->AttachToActor(Player);
	Camera->SetLoaclLocation(CameraInitLoc + FVector{ 0.f, 300.f, -300.f });
	Balloons->SetActorLocation(FVector{ -400.f, 0.f, 500.f });
}

void APlayGameMode::OnCount()
{
	Lakitu->SetActorLocation(Player->GetActorLocation() + FVector{ 100.f, 100.f, 300.f });
}

void APlayGameMode::OnPlay()
{
	Balloons->Destroy();
}

/* Fsm update function */
void APlayGameMode::Readying(float _deltaTime)
{
	Balloons->AddActorLocation({ 0.f, 20.f * _deltaTime, 0.f });
	Camera->AddRelativeLocation({ 0.f, -100.f * _deltaTime, 100.f * _deltaTime });

	FVector camLoc = Camera->GetLocalLocation();
	if (camLoc.Y <= CameraInitLoc.Y && camLoc.Z >= CameraInitLoc.Z)
	{
		GameData::GetInstance()->SetFinishState(EFinishState::FINISH_READY);
		Fsm.ChangeState(EState::WAIT_TITLE);
	}
}

void APlayGameMode::WaitingTitle(float _deltaTime)
{
	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_TITLE)
	{
		Fsm.ChangeState(EState::COUNT);
	}
}

void APlayGameMode::Counting(float _deltaTime)
{

}

void APlayGameMode::Playing(float _deltaTime)
{
	// Temp. for test
	if (UEngineInput::IsDown('Q'))
	{
		OnFinishRace();
	}
	else if (UEngineInput::IsPress('R'))
	{
		Camera->AddActorRotation({ 0.f, 100.f * _deltaTime, 0.f });
	}

	if (IsFinish)
	{
		Finishing(_deltaTime);
	}

	FVector loc = Player->GetActorLocation();
	FVector rot = Player->GetTransform().Rotation;

	//OutputDebugStringA(("playerLoc: " + std::to_string(normX) + ", " + std::to_string(normZ) + " -> " + std::to_string(x) + ", " + std::to_string(z) + "\n").c_str());

	GameData* pData = GameData::GetInstance();
	pData->SetMinimapLoc(0, loc);
	pData->SetPlayerRotation(0, rot);

	Camera->SetActorLocation(CameraInitLoc - FVector{ 0.f, 0.f, Player->GetVelocity() * .04f });
}

void APlayGameMode::Finishing(float _deltaTime)
{
	FVector playerLoc = Player->GetActorLocation();
	FVector camLoc = Camera->GetActorLocation();
	FVector camForward =Camera->GetActorForwardVector();
	playerLoc.Y = 0.f;
	camLoc.Y = 0.f;
	camForward.Y = 0.f;

	FVector sub = playerLoc - camLoc;
	camForward.Normalize();
	sub.Normalize();
	float angle = FVector::GetVectorAngleDeg(camForward, sub);
	if (!isnan(angle))
	{
		FVector crossVal = FVector::Cross(camForward, sub);
		//OutputDebugStringA(("angle: " + std::to_string(angle) + ", crossVal: " + crossVal.ToString() + "\n").c_str());

		if (crossVal.Y > 0)
		{
			Camera->AddActorRotation({ 0.f, angle, 0.f });
		}
		else
		{
			Camera->AddActorRotation({ 0.f, -angle, 0.f });
		}
	}

	float camRotY = Camera->GetActorRotation().Y;
	//OutputDebugStringA(("camRot: " + std::to_string(camRotY) + "\n").c_str());

	if (camRotY < 20 || camRotY > 140)
	{
		OnFinishRace();
	}

	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_RESULT)
	{
		UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
		if (!target->GetPostEffect(0)->IsActive)
		{
			target->GetPostEffect(0)->IsActive = true;
		}

		static float elpasedSec = 0.f;
		elpasedSec += _deltaTime;

		if (elpasedSec >= 1.f)
		{
			elpasedSec = 0.f;
			GameData::GetInstance()->SetFinishState(EFinishState::FINISH_FX);
		}
	}
	else if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_TOTAL)
	{
		if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
		{
			OutputDebugStringA("Move to next game!!!\n");
		}
	}
}

void APlayGameMode::OnFinishRace()
{
	// Temp
	if (ChangeCamIdx >= 3)
	{
		Camera->AttachToActor(Player);
		Camera->SetActorLocation({ 0.f, 100.f, 300.f });
		Camera->SetActorRotation({ 10.f, 180.f, 0.f });
	}
	else
	{
		// TODO: turn to self-driving
		if (!IsFinish)
		{
			IsFinish = true;
			Player->IsFinish = true;

			GameData::GetInstance()->SetFinishState(EFinishState::FINISH_RACING);
		}

		FVector playerLoc = Player->GetActorLocation();
		FVector forwardVec = Player->GetActorForwardVector();
		FVector rightVec = Player->GetActorRightVector() * -1;
		FVector sum = forwardVec + rightVec;
		sum *= 500;
		sum.Y = 100.f;
		Camera->SetActorRotation({ 10.f, 120.f, 0.f });
		Camera->SetActorLocation(playerLoc + sum);
		Camera->DetachFromActor();

		ChangeCamIdx++;
	}
}