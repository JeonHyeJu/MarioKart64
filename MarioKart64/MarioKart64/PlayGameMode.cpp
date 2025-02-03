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
#include "LuigiRaceway.h"
#include "RoyalRaceway.h"
#include "MarioRaceway.h"
#include "WarioStadium.h"
#include "KoopaTroopaBeach.h"
#include "SherbetLand.h"
#include "BowserCastle.h"
#include "RainbowRoad.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineRenderTarget.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineBase/EngineMath.h>

APlayGameMode::APlayGameMode()
{
	ULevel* pLevel = GetWorld();
	pLevel->CreateCollisionProfile("ItemBox");
	pLevel->CreateCollisionProfile("PLAYER");
	pLevel->CreateCollisionProfile("SHELL");
	pLevel->CreateCollisionProfile("BANANA");
	pLevel->CreateCollisionProfile("FAKE_ITEMBOX");
	pLevel->LinkCollisionProfile("PLAYER", "PLAYER");
	pLevel->LinkCollisionProfile("PLAYER", "ItemBox");
	pLevel->LinkCollisionProfile("PLAYER", "SHELL");
	pLevel->LinkCollisionProfile("PLAYER", "BANANA");
	pLevel->LinkCollisionProfile("PLAYER", "FAKE_ITEMBOX");

	Skybox = pLevel->SpawnActor<ASkybox>();
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

	InitMap();
	InitCharacters();
	InitEffects();
	InitStartPosition();

	// TODO
	Camera->GetCameraComponent()->SetZSort(0, true);

	USoundPlayer sp1 = UEngineSound::Play("StartGrandPrize.mp3");
	sp1.SetVolume(.4f);

	USoundPlayer sp2 = UEngineSound::Play("Crowd1.mp3");
	sp2.SetVolume(.1f);

	Fsm.ChangeState(EState::READY);
}

void APlayGameMode::Tick(float _deltaTime)
{
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void APlayGameMode::LevelChangeEnd()
{
	BgmSP.Stop();
}

void APlayGameMode::InitCharacters()
{
	Players.reserve(6);

	ULevel* pLevel = GetWorld();
	Player = pLevel->GetMainPawn<APlayer>();

	GameData* pData = GameData::GetInstance();
	ECircuit map = pData->GetCurMap();

	Player->InitCharacter(pData->GetPlayerCharacter());
	Player->SetMap(MapPtr.get());
	Player->InitRouteIndex(map);

	size_t playerIdx = static_cast<size_t>(pData->GetPlayerIdx());
	const std::vector<SPlayerInfo>& infos = pData->GetPlayers();
	for (size_t i = 0, size = infos.size(); i < size; ++i)
	{
		ADriver* ptr = nullptr;
		if (i == playerIdx)
		{
			ptr = Player;
		}
		else
		{
			std::shared_ptr<ADriver> driver = pLevel->SpawnActor<ADriver>();
			driver->InitCharacter(infos[i].Chracter);
			driver->SetMap(MapPtr.get());
			driver->InitRouteIndex(map);
			ptr = driver.get();
		}

		if (ptr != nullptr)
		{
			Players.push_back(ptr);
		}
	}
}

void APlayGameMode::InitEffects()
{
	UEngineRenderTarget* target = Camera->GetCameraComponent()->GetCameraTarget();
	target->AddEffect<FxShrinkEffect>();
	target->AddEffect<FxExpandEffect>();

	target->GetPostEffect(0)->IsActive = false;
}

void APlayGameMode::InitMap()
{
	GameData* pData = GameData::GetInstance();
	ECircuit mapType = pData->GetCurMap();

	switch (mapType)
	{
	case ECircuit::LUIGI_RACEWAY:
	{
		Balloons = GetWorld()->SpawnActor<ABalloons>();
		Balloons->SetActorLocation(FVector{ -400.f, 0.f, 500.f });

		MapPtr = GetWorld()->SpawnActor<ALuigiRaceway>();
		break;
	}
	case ECircuit::ROYAL_RACEWAY:
	{
		MapPtr = GetWorld()->SpawnActor<ARoyalRaceway>();
		break;
	}
	case ECircuit::MARIO_RACEWAY:
	{
		MapPtr = GetWorld()->SpawnActor<AMarioRaceway>();
		break;
	}
	case ECircuit::WARIO_STADIUM:
	{
		MapPtr = GetWorld()->SpawnActor<AWarioStadium>();
		break;
	}
	case ECircuit::KOOPA_TROOPA_BEACH:
	{
		MapPtr = GetWorld()->SpawnActor<AKoopaTroopaBeach>();
		break;
	}
	case ECircuit::SHERBET_LAND:
	{
		MapPtr = GetWorld()->SpawnActor<ASherbetLand>();
		break;
	}
	case ECircuit::BOWSERS_CASTLE:
	{
		MapPtr = GetWorld()->SpawnActor<ABowserCastle>();
		break;
	}
	case ECircuit::RAINBOW_ROAD:
	{
		Skybox->SetActive(false);
		MapPtr = GetWorld()->SpawnActor<ARainbowRoad>();
		break;
	}
	}

	// Init minimap
	float scale = MapPtr->GetScale();
	FVector initLoc = MapPtr->GetMinimapInitLoc();
	SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(mapType)->second.MapSizeInfo;
	refSize.Min *= scale;
	refSize.Max *= scale;
	pData->MapSizeInfo = SRenderInfo::MapInfos.find(mapType)->second.MapSizeInfo;
	pData->MapSizeInfo.InitLoc = initLoc;
}

void APlayGameMode::InitStartPosition()
{
	// Init start positions
	StartPosition = MapPtr->GetStartPosition();
	const float MOVE_VAL = -300.f;
	for (size_t i = 0, size = Players.size(); i < size; ++i)
	{
		Players[i]->SetActorLocation(StartPosition[i] + FVector{ 0.f, 0.f, MOVE_VAL });
	}
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

void APlayGameMode::SetPlayingLocations()
{
	FVector loc = Player->GetActorLocation();
	FVector rot = Player->GetTransform().Rotation;

	//OutputDebugStringA(("playerLoc: " + std::to_string(normX) + ", " + std::to_string(normZ) + " -> " + std::to_string(x) + ", " + std::to_string(z) + "\n").c_str());

	GameData* pData = GameData::GetInstance();
	pData->SetMinimapLoc(0, loc);
	pData->SetPlayerRotation(0, rot);

	Camera->SetLoaclLocation(CameraInitLoc - FVector{ 0.f, 0.f, Player->GetVelocity() * .04f });
}

void APlayGameMode::SetLakituLocation(float _deltaTime)
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
}

void APlayGameMode::CheckAndSetRanking(float _deltaTime, bool _isForce)
{
	static float elapsedSecs = 0.f;

	elapsedSecs += _deltaTime;

	if (_isForce || elapsedSecs > .1f)
	{
		elapsedSecs = 0.f;
		GameData* pData = GameData::GetInstance();

		size_t size = Players.size();
		std::vector<int> rank;
		std::vector<int> scores;
		rank.reserve(size);
		scores.reserve(size);

		for (size_t i = 0; i < size; ++i)
		{
			ADriver* driver = Players[i];
			int lap = driver->GetLap();
			int idx = driver->GetRouteIdx();
			int score = lap * 10000 + idx;
			if (Players[i]->GetIsCheckingLap() == false)
			{
				score = 0;
			}
			scores.push_back(score);
		}

		for (size_t i = 0; i < size; ++i)
		{
			int ranking = 0;
			float distI = Players[i]->GetDistFromNextRoute();
			for (size_t j = 0; j < size; ++j)
			{
				if (i == j) continue;
				if (scores[j] > scores[i])
				{
					ranking++;
				}
				else if (scores[j] == scores[i])
				{
					float distJ = Players[j]->GetDistFromNextRoute();
					if (distI > distJ)
					{
						ranking++;
					}
				}
			}
			Players[i]->Rank = ranking;
			rank.push_back(ranking);
		}

		pData->SetRankings(rank);
	}
}

/* Fsm start function */
void APlayGameMode::OnGetReady()
{
	Camera->AttachToActor(Player);
	Camera->SetLoaclLocation(CameraInitLoc + FVector{ 0.f, 300.f, -300.f });
}

void APlayGameMode::OnCount()
{
	if (Balloons != nullptr)
	{
		Balloons->Destroy();
		Balloons = nullptr;
	}

	Lakitu->SetActive(true);
	Lakitu->SetActorLocation(Player->GetActorLocation() + FVector{ 100.f, 600.f, 600.f });

	SetPlayingLocations();
	CheckAndSetRanking(0.f, true);
}

void APlayGameMode::OnPlay()
{
	BgmSP = UEngineSound::Play("02.RacewayAndWarioStadium.mp3");
	BgmSP.SetVolume(.4f);

	for (ADriver* ptr : Players)
	{
		ptr->SetStart(true);
	}
}

void APlayGameMode::OnFinish()
{
	SetCamFinishRot();
	Camera->DetachFromActor();

	BgmSP.Stop();
	USoundPlayer sp = UEngineSound::Play("17.1stPlaceFanfare.mp3");
	sp.SetVolume(.5f);

	GameData::GetInstance()->SetFinishState(EFinishState::FINISH_RACING);
}

void APlayGameMode::OnWaitUIResult()
{
	Camera->AttachToActor(Player);
	Camera->SetActorLocation({ 0.f, 100.f, 300.f });
	Camera->SetActorRotation({ 10.f, 180.f, 0.f });

	Player->HideDefaultRenderer();

	BgmResultSP = UEngineSound::Play("19.1st-4thPlaceResults.mp3");
	BgmResultSP.SetVolume(.5f);
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
	int completeCnt = 0;
	for (size_t i = 0, size = Players.size(); i < size; ++i)
	{
		float z = Players[i]->GetActorLocation().Z;
		float dstZ = StartPosition[i].Z;
		if (z < dstZ)
		{
			float addVal = 100.f * _deltaTime;
			if (z + addVal > dstZ)
			{
				addVal = dstZ - z;
			}
			Players[i]->AddActorLocation({ 0.f, 0.f, addVal });
		}
		else
		{
			++completeCnt;
		}
	}

	if (completeCnt == Players.size())
	{
		if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_OPEN)
		{
			Fsm.ChangeState(EState::COUNT);
		}
	}
}

void APlayGameMode::Counting(float _deltaTime)
{
	static int state = 0;
	FVector dstLoc = Player->GetActorLocation() + FVector{ 100.f, 100.f, 100.f };
	FVector loc = Lakitu->GetActorLocation();
	if (loc.Y > dstLoc.Y && loc.Z > dstLoc.Z)
	{
		Lakitu->AddActorLocation({ -50.f * _deltaTime, -300.f * _deltaTime, -300.f * _deltaTime });
		return;
	}

	if (state == 0)
	{
		state = 1;

		USoundPlayer sp1 = UEngineSound::Play("StartSignal.mp3");
		sp1.SetVolume(1.f);

		Lakitu->Launch();
	}
	else if (state == 1)
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
	// for test
	if (UEngineInput::IsDown('Q'))
	{
		Player->EndLap();
		Fsm.ChangeState(EState::FINISH);
		return;
	}

	if (Player->GetIsFinished())
	{
		Fsm.ChangeState(EState::FINISH);
		return;
	}

	SetPlayingLocations();
	SetLakituLocation(_deltaTime);
	CheckAndSetRanking(_deltaTime);
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
				BgmResultSP.Stop();
			}
		}
	}
}