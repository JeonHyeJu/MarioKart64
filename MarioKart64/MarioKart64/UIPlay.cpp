#include "PreCompile.h"
#include "UIPlay.h"
#include "GameData.h"
#include "CData.h"
#include "CGlobal.h"
#include "TextWidget.h"
#include <EngineCore/ImageWidget.h>
#include "TextWidget.h"

AUIPlay::AUIPlay()
{
	Fsm.CreateState(EState::IDLE, std::bind(&AUIPlay::Idleing, this, std::placeholders::_1), std::bind(&AUIPlay::OnIdle, this));
	Fsm.CreateState(EState::RACING, std::bind(&AUIPlay::Racing, this, std::placeholders::_1), std::bind(&AUIPlay::OnRacing, this));
	Fsm.CreateState(EState::RESULT, std::bind(&AUIPlay::ShowingResult, this, std::placeholders::_1), std::bind(&AUIPlay::OnShowResult, this));
	Fsm.CreateState(EState::WAIT, std::bind(&AUIPlay::Waiting, this, std::placeholders::_1), std::bind(&AUIPlay::OnWait, this));
	Fsm.CreateState(EState::TOTAL, std::bind(&AUIPlay::ShowingTotal, this, std::placeholders::_1), std::bind(&AUIPlay::OnShowTotal, this));
}

AUIPlay::~AUIPlay()
{
}

void AUIPlay::BeginPlay()
{
	AHUD::BeginPlay();

	InitMinimap();
	InitPlayerRank();
	InitElpasedTime();
	StartTimer();
	InitHighRank();
	InitItem();
	InitMinimapLoc();
	InitLap();
	InitTexts();

	// Temp
	SetHighRankUI();

	// for test
	/*{
		std::shared_ptr<UFontWidget> Widget = CreateWidget<UFontWidget>(1);

		Widget->SetWorldLocation({ 200, 300 });
		Widget->SetFont("Arial", 40.f, UColor{ 255, 0, 0, 255 });
		Widget->SetText("ABCDEFGHIJKLMNOP");
	}*/

	Fsm.ChangeState(EState::IDLE);
}

void AUIPlay::Tick(float _deltaTime)
{
	AHUD::Tick(_deltaTime);

	if (!IsStartCount) return;

	Fsm.Update(_deltaTime);
}

void AUIPlay::Reset()
{
	ShowingLap = 1;
	LapC->SetSprite(LAP_SPRITE, ShowingLap);
}

void AUIPlay::InitMinimap()
{
	GameData* gameData = GameData::GetInstance();
	ECircuit type = gameData->GetCurMap();
	int idx = 0;

	Minimap = CreateWidget<UImageWidget>(-1);

	switch (type)
	{
	case ECircuit::LUIGI_RACEWAY:
		idx = 91;
		break;
	case ECircuit::ROYAL_RACEWAY:
		idx = 101;
		break;
	case ECircuit::KOOPA_TROOPA_BEACH:
		idx = 93;
		break;
	case ECircuit::MARIO_RACEWAY:
		idx = 98;
		break;
	case ECircuit::WARIO_STADIUM:
		idx = 99;
		break;
	case ECircuit::SHERBET_LAND:
		idx = 100;
		break;
	case ECircuit::BOWSERS_CASTLE:
		idx = 102;
		break;
	case ECircuit::RAINBOW_ROAD:
		idx = 107;
		break;
	}
	Minimap->SetSprite("TrackIcons.png", idx);
	Minimap->SetAutoScaleRatio(3.f);
	Minimap->SetWorldLocation({ 450, -250 });
	MinimapSizeInfo = Minimap->GetRealScaleOfSprite();
}

void AUIPlay::InitPlayerRank()
{
	GameData* gameData = GameData::GetInstance();
	uint8_t playerCnt = gameData->GetPlayerCnt();

	PlayerRanking = CreateWidget<UImageWidget>(-1);

	PlayerRanking->SetSprite(FONT_SPRITE, RANK_1ST_SPRITE_IDX+7);
	PlayerRanking->SetAutoScaleRatio(1.f);
	PlayerRanking->SetWorldLocation({ -450, -250 });

	SetPlayerRankColor(playerCnt);
}

void AUIPlay::SetPlayerRankColor(uint8_t _val)
{
	float val = RANK_MUL_COLOR * _val;
	PlayerRanking->ColorData.MulColor = { 1.f, 1 - val, 0.f, 1.f };
}

void AUIPlay::InitElpasedTime()
{
	TimeT = CreateWidget<UImageWidget>(-1);
	TimeC = CreateWidget<UImageWidget>(-1);
	TimeS = CreateWidget<UImageWidget>(-1);

	TimeT->SetSprite("Timer.png", 0);
	TimeT->SetAutoScaleRatio(3.f);
	TimeT->SetWorldLocation({ 200, 330 });

	TimeC->SetSprite("Timer.png", 13);
	TimeC->SetAutoScaleRatio(3.f);
	TimeC->SetWorldLocation({ 355, 322 });

	TimeS->SetSprite("Timer.png", 12);
	TimeS->SetAutoScaleRatio(3.f);
	TimeS->SetWorldLocation({ 420, 330 });

	TimeList.reserve(TIME_CNT);
	float width = -1.f;
	const float INIT_X = 300.f;
	const float MARGIN = 1.f;
	float addMargin = 0.f;
	for (int i = 0; i < TIME_CNT; ++i)
	{
		std::shared_ptr<UImageWidget> ptr = CreateWidget<UImageWidget>(-1);
		ptr->SetSprite("Timer.png", ZERO);
		ptr->SetAutoScaleRatio(3.f);

		if (width < 0)
		{
			FVector scale = ptr->GetRealScaleOfSprite();
			width = scale.X;
		}
		if (i == 2 || i == 4)
		{
			addMargin += 20.f;
		}
		
		ptr->SetWorldLocation({ INIT_X + i * (width + MARGIN) + addMargin, 330.f });

		TimeList.push_back(ptr.get());
	}
}

void AUIPlay::InitHighRank()
{
	HighRankPlayers.reserve(HIGH_RANK_CNT);

	const float INIT_Y = 300.f;
	const float MARGIN = 10.f;
	float width = -1.f;
	float height = -1.f;
	for (int i = 0; i < HIGH_RANK_CNT; ++i)
	{
		// Init players image
		std::shared_ptr<UImageWidget> ptr = CreateWidget<UImageWidget>(-1);
		ptr->SetSprite(RANK_PLAYER_SPRITE, 0);
		ptr->SetAutoScaleRatio(2.f);
		ptr->ColorData.MulColor = { 1.f, 1.f, 1.f, 0.75f };

		if (height < 0)
		{
			FVector scale = ptr->GetRealScaleOfSprite();
			width = scale.X;
			height = scale.Y;
		}

		ptr->SetWorldLocation({ -550.f, INIT_Y - i * (height + MARGIN) });
		HighRankPlayers.push_back(ptr.get());

		// Init numbers
		std::shared_ptr<UImageWidget> ptrN = CreateWidget<UImageWidget>(0);
		ptrN->SetSprite(FONT_SPRITE, 1);
		ptrN->SetAutoScaleRatio(1.f);
		ptrN->SetWorldLocation(ptr->GetWorldLocation() + FVector{ -5.f, -height + 25.f, 0.f });
		HighRankNumbers.push_back(ptrN.get());
	}
}

void AUIPlay::InitItem()
{
	PlayerItem = CreateWidget<UImageWidget>(-1);
	PlayerItem->SetSprite(ITEM_SPRITE, static_cast<int>(EItemType::SIZE));
	PlayerItem->SetAutoScaleRatio(3.f);
	PlayerItem->SetWorldLocation({ 0.f, 290.f });
}

void AUIPlay::InitMinimapLoc()
{
	int size = GameData::MAX_PLAYER_CNT;
	MinimapLocs.reserve(size);

	// Temp
	float initX = 465.f;
	float initY = -260.f;
	for (int i = 0; i < size; ++i)
	{
		std::shared_ptr<UImageWidget> ptr = CreateWidget<UImageWidget>(0);
		ptr->SetSprite(ITEM_SPRITE, MINIMAP_CAR_INIT_IDX);
		ptr->SetAutoScaleRatio(2.f);
		ptr->SetWorldLocation(Minimap->GetWorldLocation());
		MinimapLocs.push_back(ptr.get());

		break;
	}
}

void AUIPlay::InitLap()
{
	float winHH = CGlobal::FWINDOW_H * .5f;

	LapT = CreateWidget<UImageWidget>(0);
	LapT->SetSprite(LAP_SPRITE, 3);
	LapT->SetAutoScaleRatio(.5f);
	LapT->SetWorldLocation({ -270.f, winHH - 100.f, 0.f });

	LapC = CreateWidget<UImageWidget>(0);
	LapC->SetSprite(LAP_SPRITE, 0);
	LapC->SetAutoScaleRatio(.5f);
	LapC->SetWorldLocation({ -170.f, winHH - 87.f, 0.f });
}

void AUIPlay::InitTexts()
{
	const float MARGIN_H = -50.f;
	{
		FVector initLoc{ 50.f, 300.f };
		std::vector<std::string> initTexts = {
			"RESULTS",
			"ROUND1",
			"1.MARIO   00'00\"00",
			"2.MARIO   00'00\"00",
			"3.MARIO   00'00\"00",
			"4.MARIO   00'00\"00",
		};

		for (size_t i = 0, size = initTexts.size(); i < size; ++i)
		{
			ATextWidget* ptr = GetWorld()->SpawnActor<ATextWidget>().get();
			AddChild(ptr);
			UpperTexts.push_back(ptr);
			UpperTexts[i]->SetText(initTexts[i]);
			UpperTexts[i]->Move(initLoc + FVector{ 0.f, MARGIN_H } * static_cast<float>(i));
		}

		UpperTexts[0]->SetScaleRatio(4.f);
		UpperTexts[0]->Move(initLoc + FVector{ 100.f, 0.f });
		//UpperTexts[0]->SetColor({ 1.f, 0.f, 0.f, 1.f });
		UpperTexts[0]->SetAutoColor(true);

		UpperTexts[1]->Move(initLoc + FVector{ 175.f, MARGIN_H - 4.f });
	}

	{
		FVector initLoc{ -100.f, -100.f };
		std::vector<std::string> initTexts = {
			"5.MARIO   00'00\"00",
			"6.MARIO   00'00\"00",
			"7.MARIO   00'00\"00",
			"8.MARIO   00'00\"00",
			"MUSHROOM CUP 50#",
		};

		for (size_t i = 0, size = initTexts.size(); i < size; ++i)
		{
			ATextWidget* ptr = GetWorld()->SpawnActor<ATextWidget>().get();
			AddChild(ptr);
			LowerTexts.push_back(ptr);
			LowerTexts[i]->SetText(initTexts[i]);
			LowerTexts[i]->Move(initLoc + FVector{ 0.f, MARGIN_H } * static_cast<float>(i));
		}

		LowerTexts[LowerTexts.size() - 1]->SetScaleRatio(2.f);
	}
}

void AUIPlay::CountTimer(float _deltaTime)
{
	ElapsedSecs += _deltaTime;
	SetTimerUI(ElapsedSecs);
}

void AUIPlay::ResetTimer()
{
	ElapsedSecs = 0.f;
	SetTimerUI(0);
}

void AUIPlay::SetTimerUI(float _secs)
{
	static int MIN_SEC = 60;
	static int BASE_10 = 10;

	int m = static_cast<int>(_secs / MIN_SEC);
	_secs -= m * MIN_SEC;

	int s = static_cast<int>(trunc(_secs));
	_secs -= s;

	{
		int n10 = m / BASE_10;
		int n1 = m - (n10 * BASE_10);

		TimeList[0]->SetSprite("Timer.png", n10 + ZERO);
		TimeList[1]->SetSprite("Timer.png", n1 + ZERO);
	}

	{
		int n10 = s / BASE_10;
		int n1 = s - (n10 * BASE_10);

		TimeList[2]->SetSprite("Timer.png", n10 + ZERO);
		TimeList[3]->SetSprite("Timer.png", n1 + ZERO);
	}

	{
		int ms = static_cast<int>(trunc(_secs * 100));
		int n10 = static_cast<int>(ms / BASE_10);
		int n1 = static_cast<int>(ms - (n10 * BASE_10));

		TimeList[4]->SetSprite("Timer.png", n10 + ZERO);
		TimeList[5]->SetSprite("Timer.png", n1 + ZERO);
	}
}

void AUIPlay::SetHighRankUI()
{
	static const int RANK_SIZE = 4;
	int rank[RANK_SIZE];

	GameData* gameData = GameData::GetInstance();
	gameData->GetHighRankPlayersIdx(rank, RANK_SIZE);

	for (int i = 0; i < RANK_SIZE; ++i)
	{
		// Temp
		//ECharacter characetrType = static_cast<ECharacter>(rank[i]);
		HighRankPlayers[i]->SetSprite(RANK_PLAYER_SPRITE, rank[i]);
		HighRankNumbers[i]->SetSprite(FONT_SPRITE, RANK_HIGH_SPRITE_IDX + i);
	}
}

void AUIPlay::SetItemUI()
{
	int idx = static_cast<int>(GameData::GetInstance()->GetItem(0));
	if (ItemIdx == idx) return;

	ItemIdx = idx;
	PlayerItem->SetSprite(ITEM_SPRITE, ItemIdx);
}

void AUIPlay::SetMinimapLoc()
{
	GameData* pData = GameData::GetInstance();
	float4 playerLoc = pData->GetMinimapLoc(0);
	float4 playerRot = pData->GetPlayerRotation(0);

	float subX = pData->MapSizeInfo.Max.X - pData->MapSizeInfo.Min.X;
	float subZ = pData->MapSizeInfo.Max.Z - pData->MapSizeInfo.Min.Z;
	float normX = (playerLoc.X - pData->MapSizeInfo.Min.X) / subX;
	float normZ = (playerLoc.Z - pData->MapSizeInfo.Min.Z) / subZ;
	float x = normX * MinimapSizeInfo.X;
	float z = normZ * MinimapSizeInfo.Y;
	
	//OutputDebugStringA(("playerLoc: " + std::to_string(normX) + ", " + std::to_string(normZ) + " -> " + std::to_string(x) + ", " + std::to_string(z) + "\n").c_str());

	FVector mapLoc = Minimap->GetWorldLocation();
	MinimapLocs[0]->SetWorldLocation(
		mapLoc + 
		FVector{ pData->MapSizeInfo.InitLoc.X + x, pData->MapSizeInfo.InitLoc.Z + z
	});
	MinimapLocs[0]->SetRotation(FVector{ 0.f, 0.f, -playerRot.Y });
}

void AUIPlay::SetPlayUIVisible(bool _val)
{
	Minimap->SetActive(_val);
	PlayerRanking->SetActive(_val);

	LapT->SetActive(_val);
	LapC->SetActive(_val);

	TimeT->SetActive(_val);
	TimeC->SetActive(_val);
	TimeS->SetActive(_val);

	for (size_t i = 0, size = TimeList.size(); i < size; ++i)
	{
		TimeList[i]->SetActive(_val);
	}

	for (size_t i = 0, size = HighRankNumbers.size(); i < size; ++i)
	{
		HighRankNumbers[i]->SetActive(_val);
	}
	for (size_t i = 0, size = HighRankPlayers.size(); i < size; ++i)
	{
		HighRankPlayers[i]->SetActive(_val);
	}
	
	PlayerItem->SetActive(_val);
}

/* Fsm start function */
void AUIPlay::OnIdle()
{
	SetPlayUIVisible(false);
}

void AUIPlay::OnRacing()
{
	SetPlayUIVisible(true);
}

void AUIPlay::OnShowResult()
{
	SetPlayUIVisible(false);
}

void AUIPlay::OnWait()
{
	GameData::GetInstance()->SetFinishState(EFinishState::FINISH_RESULT);
}

void AUIPlay::OnShowTotal()
{
}

/* Fsm update function */
void AUIPlay::Idleing(float _deltaTime)
{
	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_READY)
	{
		Fsm.ChangeState(EState::RACING);
	}
}

void AUIPlay::Racing(float _deltaTime)
{
	CountTimer(_deltaTime);
	SetItemUI();
	SetMinimapLoc();

	GameData* pData = GameData::GetInstance();

	// User
	int lap = pData->GetPlayerLap(0);
	if (ShowingLap != lap)
	{
		ShowingLap = lap;
		LapC->SetSprite(LAP_SPRITE, lap - 1);
	}

	if (pData->GetFinishState() == EFinishState::FINISH_RACING)
	{
		Fsm.ChangeState(EState::RESULT);
	}
}

void AUIPlay::ShowingResult(float _deltaTime)
{
	// Somthing..
	// after execute this

	Fsm.ChangeState(EState::WAIT);
}

void AUIPlay::Waiting(float _deltaTime)
{
	if (GameData::GetInstance()->GetFinishState() == EFinishState::FINISH_FX)
	{
		Fsm.ChangeState(EState::TOTAL);
	}
}

void AUIPlay::ShowingTotal(float _deltaTime)
{

}
