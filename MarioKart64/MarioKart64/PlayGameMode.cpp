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
#include "UIPlay.h"
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
	
	Camera = GetWorld()->GetMainCamera();
	Camera->GetCameraComponent()->GetCameraTarget()->SetClearColor({ 0.f, 0.f, 0.f, 1.f });

	Fsm.CreateState(EState::READY, std::bind(&APlayGameMode::Readying, this, std::placeholders::_1), std::bind(&APlayGameMode::OnGetReady, this));
	Fsm.CreateState(EState::WAIT_UI_OPEN, std::bind(&APlayGameMode::WaitingUIOpen, this, std::placeholders::_1));
	Fsm.CreateState(EState::COUNT, std::bind(&APlayGameMode::Counting, this, std::placeholders::_1), std::bind(&APlayGameMode::OnCount, this));
	Fsm.CreateState(EState::PLAY, std::bind(&APlayGameMode::Playing, this, std::placeholders::_1), std::bind(&APlayGameMode::OnPlay, this));
	Fsm.CreateState(EState::FINISH, std::bind(&APlayGameMode::Finishing, this, std::placeholders::_1), std::bind(&APlayGameMode::OnFinish, this));
	Fsm.CreateState(EState::WAIT_UI_RESULT, std::bind(&APlayGameMode::WaitingUIResult, this, std::placeholders::_1), std::bind(&APlayGameMode::OnWaitUIResult, this));
	Fsm.CreateState(EState::WAIT_KEY, std::bind(&APlayGameMode::WaitingKey, this, std::placeholders::_1), std::bind(&APlayGameMode::OnWaitKey, this));
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();

	InitCharacters();
	InitEffects();
	InitMap();

	// TODO
	Camera->GetCameraComponent()->SetZSort(0, true);

	Fsm.ChangeState(EState::READY);
}

void APlayGameMode::Tick(float _deltaTime)
{
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void APlayGameMode::InitCharacters()
{
	ULevel* pLevel = GetWorld();
	Player = pLevel->GetMainPawn<APlayer>();

	GameData* pData = GameData::GetInstance();
	Player->InitCharacter(pData->GetPlayerCharacter());
	Player->SetMap(MapPtr.get());

	uint8_t playerIdx = pData->GetPlayerIdx();
	//uint8_t size = pData->GetPlayerCnt();
	uint8_t size = 1;
	for (uint8_t i = 0; i < size; ++i)
	{
		if (i == playerIdx)
		{
			Players[i] = Player;
		}
		else
		{
			std::shared_ptr<ADriver> driver = pLevel->SpawnActor<ADriver>();
			driver->InitCharacter(static_cast<ECharacter>(i));
			driver->SetMap(MapPtr.get());
			Players[i] = driver.get();
		}
	}
}

void APlayGameMode::InitEffects()
{
	UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
	target->AddEffect<FxShrinkEffect>();
	target->AddEffect<FxExpandEffect>();

	target->GetPostEffect(0)->IsActive = false;
	//target->GetPostEffect(1)->IsActive = false;
}

void APlayGameMode::InitMap()
{
	GameData* pData = GameData::GetInstance();
	ECircuit mapType = pData->GetCurMap();

	float scale = 1.f;
	FVector initLoc = FVector::ZERO;
	MapPtr->Init(mapType);
	switch (mapType)
	{
	case ECircuit::LUIGI_RACEWAY:
	{
		Balloons = GetWorld()->SpawnActor<ABalloons>();
		Balloons->SetActorLocation(FVector{ -400.f, 0.f, 500.f });

		MapPtr->SetActorLocation({ 0.0f, 0.f, 0.f });
		MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

		Player->SetActorLocation({ -400.0f, -100.0f, 300.0f });

		// Temp
		//Players[1]->SetActorLocation({ -600.0f, -100.0f, 300.0f });

		//Player->SetActorLocation({ -400.0f, -60.0f, 7000.0f });

		//Player->SetActorLocation({ -1200.f, -782.f, 2641.f });
		//Player->SetActorRotation({ 0.f, 180.f, 0.f });

		//Player->SetActorLocation({ -3800.0f, -60.0f, -6600.f });
		//Player->SetActorRotation({ 0.f, 90.f, 0.f });

		scale = 4000.f;
		initLoc = { 50.f, 0.f, -200.f };
		break;
	}
	case ECircuit::KOOPA_TROOPA_BEACH:
	{
		Player->SetActorLocation({ 0.0f, 20.0f, -1000.0f });

		//Player->SetActorLocation({ -395.0f, 15.0f, 4370});

		//Player->SetActorLocation({ -7911.f, 15.0f, -2318.f });

		//Player->SetActorLocation({ -6686.f, 15.0f, -12073.f });

		scale = 50.f;
		initLoc = { -52.f, 0.f, -100.f };
		break;
	}
	case ECircuit::MARIO_RACEWAY:
	{
		MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

		Player->SetActorLocation({ 0.0f, 10.0f, 0.0f });

		//Player->SetActorLocation({ 1734.f, 10.0f, 4060.0f });

		//Player->SetActorLocation({ 10238.f, 10.0f, 318.0f });

		//Player->SetActorLocation({ 8657.f, 10.0f, -4202.0f });

		scale = 4500.f;
		initLoc = { -250.f, 0.f, -55.f };
		break;
	}
	case ECircuit::WARIO_STADIUM:
	{
		MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

		Player->SetActorLocation({ 5000.0f, 10.0f, 0.0f });

		//Player->SetActorLocation({ -3405.0f, 10.0f, 660.0f });
		//Player->SetActorRotation({ 0.f, 180.f, 0.f });

		//Player->SetActorLocation({ 1677.0f, 10.0f, 7043.0f });
		//Player->SetActorRotation({ 0.f, 90.f, 0.f });

		//Player->SetActorLocation({ 18.0f, 10.0f, -180.0f });
		//Player->SetActorRotation({ 0.f, 180.f, 0.f });

		scale = 1000.f;
		initLoc = { -64.f, 0.f, -120.f };
		break;
	}
	case ECircuit::SHERBET_LAND:
	{
		MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

		Player->SetActorLocation({ 4719.f, 150.0f, -3392.0f });

		//Player->SetActorLocation({ -7407.f, 150.0f, -2594.0f });
		//Player->SetActorRotation({ 0.f, 180.0f, 0.f });

		//Player->SetActorLocation({ -9188.f, -300.0f, -10933.0f });

		//Player->SetActorLocation({ -1849.f, 150.0f, -2965.0f });

		scale = 50.f;
		initLoc = { -53.f, 0.f, -58.f };
		break;
	}
	case ECircuit::ROYAL_RACEWAY:
	{
		MapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

		Player->SetActorLocation({ -50.0f, 100.0f, 700.0f });

		//Player->SetActorLocation({ -1500.0f, 100.0f, 8485.0f });

		//Player->SetActorLocation({ 2373.0f, 100.0f, 10796.0f });

		//Player->SetActorLocation({ -8662.0f, 0.0f, 2019.0f });

		scale = 4.f;
		initLoc = { -90.f, 0.f, -80.f };
		break;
	}
	case ECircuit::BOWSERS_CASTLE:
	{
		MapPtr->SetActorRotation({ 0.f, 180.f, 0.f });

		Player->SetActorLocation({ 0.f, 0.f, 0.f });

		//Player->SetActorLocation({ -82.f, 0.f, 6059.f });

		//Player->SetActorLocation({ 4909.f, 0.f, 3198.f });

		//Player->SetActorLocation({ 4838.f, 0.f, -1705.f });
		//Player->SetActorRotation({ 0.f, -90.f, 0.f });

		scale = 4000.f;
		initLoc = { -196.f, 0.f, -200.f };
		break;
	}
	case ECircuit::RAINBOW_ROAD:
	{
		Skybox->SetActive(false);

		Player->SetActorLocation({ 900.f, 5050.f, -1000.f });

		//Player->SetActorLocation({ 1509.f, 2435.f, 24884.f });

		//Player->SetActorLocation({ -605.f, 2155.f, 15872.f });

		//Player->SetActorLocation({ -6979.f, -80.f, 4569.f });

		//Player->SetActorLocation({ -8419.f, 1035.f, -5402.f });

		//Player->SetActorLocation({ -7871.f, 1155.f, -11415.f });

		//Player->SetActorLocation({ 7310.f, 1155.f, -10154.f });

		//Player->SetActorLocation({ -364.f, 2137.f, -13920.f });

		//Player->SetActorLocation({ 2654.f, 4050.f, -12532.f });

		scale = 100.f;
		initLoc = { -83.f, 0.f, -140.f };
		break;
	}
	}

	MapPtr->SetActorRelativeScale3D({ scale, scale, scale });
	
	// Init minimap
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(mapType)->second.MapSizeInfo;
	refSize.Min *= scale;
	refSize.Max *= scale;
	pData->MapSizeInfo = SRenderInfo::MapInfos.find(mapType)->second.MapSizeInfo;
	pData->MapSizeInfo.InitLoc = initLoc;
}

void APlayGameMode::SetCamFinishRot()
{
	FVector playerLoc = Player->GetActorLocation();
	FVector forwardVec = Player->GetActorForwardVector();
	FVector rightVec = Player->GetActorRightVector() * -1;
	FVector sum = forwardVec + rightVec;
	sum *= 500;
	sum.Y = 100.f;
	Camera->SetActorRotation({ 10.f, 120.f, 0.f });
	Camera->SetActorLocation(playerLoc + sum);
}

/* Fsm start function */
void APlayGameMode::OnGetReady()
{
	Camera->AttachToActor(Player);
	Camera->SetLoaclLocation(CameraInitLoc + FVector{ 0.f, 300.f, -300.f });
}

void APlayGameMode::OnCount()
{
	Lakitu->SetActive(true);
	Lakitu->SetActorLocation(Player->GetActorLocation() + FVector{ 100.f, 600.f, 600.f });
}

void APlayGameMode::OnPlay()
{
	if (Balloons != nullptr)
	{
		Balloons->Destroy();
		Balloons = nullptr;
	}

	int size = ARRAYSIZE(Players);
	for (int i = 0; i < size; ++i)
	{
		if (Players[i] != nullptr)
		{
			Players[i]->SetStart(true);
		}
	}
}

void APlayGameMode::OnFinish()
{
	// TODO: turn to self-driving
	
	SetCamFinishRot();
	Camera->DetachFromActor();

	GameData::GetInstance()->SetFinishState(EFinishState::FINISH_RACING);
}

void APlayGameMode::OnWaitUIResult()
{
	Camera->AttachToActor(Player);
	Camera->SetActorLocation({ 0.f, 100.f, 300.f });
	Camera->SetActorRotation({ 10.f, 180.f, 0.f });
}

void APlayGameMode::OnWaitKey()
{

}

/* Fsm update function */
void APlayGameMode::Readying(float _deltaTime)
{
	if (Balloons != nullptr)
	{
		Balloons->AddActorLocation({ 0.f, 20.f * _deltaTime, 0.f });
	}
	Camera->AddRelativeLocation({ 0.f, -100.f * _deltaTime, 100.f * _deltaTime });

	FVector camLoc = Camera->GetLocalLocation();
	if (camLoc.Y <= CameraInitLoc.Y && camLoc.Z >= CameraInitLoc.Z)
	{
		GameData::GetInstance()->SetFinishState(EFinishState::FINISH_READY);
		Fsm.ChangeState(EState::WAIT_UI_OPEN);
	}
}

void APlayGameMode::WaitingUIOpen(float _deltaTime)
{
	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_OPEN)
	{
		Fsm.ChangeState(EState::COUNT);
	}
}

void APlayGameMode::Counting(float _deltaTime)
{
	static int state = 0;
	FVector loc = Lakitu->GetActorLocation();
	if (!(loc.Y < -100.f && loc.Z < 400.f))
	{
		Lakitu->AddActorLocation({ -50.f * _deltaTime, -300.f * _deltaTime, -300.f * _deltaTime });
		state = 1;
		return;
	}

	if (state == 1)
	{
		state = 2;
		Lakitu->Launch();
	}
	else if (state == 2)
	{
		if (Lakitu->IsAnimEnd())
		{
			state = 0;
			GameData::GetInstance()->SetFinishState(EFinishState::FINISH_COUNT);
			Fsm.ChangeState(EState::PLAY);
		}
	}
}

void APlayGameMode::Playing(float _deltaTime)
{
	if (Lakitu->IsActive())
	{
		Lakitu->AddActorLocation({ -100.f * _deltaTime, 100.f * _deltaTime, -150.f * _deltaTime });
		FVector loc = Lakitu->GetActorLocation();
		if (loc.Y >= 60.f && loc.Z < 155.f)
		{
			Lakitu->SetActive(false);
		}
	}

	// Temp. for test
	if (UEngineInput::IsDown('Q'))
	{
		Fsm.ChangeState(EState::FINISH);
		return;
	}

	FVector loc = Player->GetActorLocation();
	FVector rot = Player->GetTransform().Rotation;

	//OutputDebugStringA(("playerLoc: " + std::to_string(normX) + ", " + std::to_string(normZ) + " -> " + std::to_string(x) + ", " + std::to_string(z) + "\n").c_str());

	GameData* pData = GameData::GetInstance();
	pData->SetMinimapLoc(0, loc);
	pData->SetPlayerRotation(0, rot);

	Camera->SetLoaclLocation(CameraInitLoc - FVector{ 0.f, 0.f, Player->GetVelocity() * .04f });
}

void APlayGameMode::Finishing(float _deltaTime)
{
	static float elapsedSec = 0.f;
	static int turnCount = 0;

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
		turnCount++;
		SetCamFinishRot();
		if (turnCount > 2)
		{
			turnCount = 0;
			Fsm.ChangeState(EState::WAIT_UI_RESULT);
		}
	}

	elapsedSec += _deltaTime;
	if (elapsedSec >= 3.f)
	{
		elapsedSec = 0.f;
		Fsm.ChangeState(EState::WAIT_UI_RESULT);
	}
}

void APlayGameMode::WaitingUIResult(float _deltaTime)
{
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
			Fsm.ChangeState(EState::WAIT_KEY);
		}
	}
}

void APlayGameMode::WaitingKey(float _deltaTime)
{
	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_TOTAL)
	{
		if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
		{
			ECircuit nextMap = GameData::GetInstance()->NextMap();
			if (nextMap == ECircuit::END)
			{
				UEngineCore::OpenLevel("SelectLevel");
			}
			else
			{
				UEngineCore::ResetLevel<APlayGameMode, APlayer, AUIPlay>("PlayLevel");
			}
		}
	}
}