#include "PreCompile.h"
#include "UIPlay.h"
#include "GameData.h"
#include "CData.h"
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>

AUIPlay::AUIPlay()
{

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

	// Temp
	SetHighRankUI();
}

void AUIPlay::Tick(float _deltaTime)
{
	AHUD::Tick(_deltaTime);

	if (!IsStartCount) return;

	CountTimer(_deltaTime);
	SetItemUI();
}

void AUIPlay::InitMinimap()
{
	GameData* gameData = GameData::GetInstance();
	uint8_t mapIdx = gameData->GetMapIdx();

	Minimap = CreateWidget<UImageWidget>(-1);

	Minimap->SetSprite("TrackIcons.png", 101);	// Temp
	Minimap->SetAutoScaleRatio(3.f);
	Minimap->SetWorldLocation({ 450, -250 });
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
	TimeT->SetWorldLocation({ 200, 250 });

	TimeC->SetSprite("Timer.png", 13);
	TimeC->SetAutoScaleRatio(3.f);
	TimeC->SetWorldLocation({ 355, 242 });

	TimeS->SetSprite("Timer.png", 12);
	TimeS->SetAutoScaleRatio(3.f);
	TimeS->SetWorldLocation({ 420, 250 });

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
		
		ptr->SetWorldLocation({ INIT_X + i * (width + MARGIN) + addMargin, 250.f });

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
		ptrN->SetAutoScaleRatio(1.5f);
		ptrN->SetWorldLocation(ptr->GetWorldLocation() + FVector{ -5.f, -height + 25.f, 0.f });
		HighRankNumbers.push_back(ptrN.get());
	}
}

void AUIPlay::InitItem()
{
	PlayerItem = CreateWidget<UImageWidget>(-1);
	PlayerItem->SetSprite(ITEM_SPRITE, static_cast<int>(EItemType::SIZE));
	PlayerItem->SetAutoScaleRatio(3.f);
	PlayerItem->SetWorldLocation({ 0.f, 250.f });
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
		ptr->SetWorldLocation({ initX, initY });
		MinimapLocs.push_back(ptr.get());
		break;
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
		HighRankNumbers[i]->SetSprite(FONT_SPRITE, i + 1);
	}
}

void AUIPlay::SetItemUI()
{
	int idx = static_cast<int>(GameData::GetInstance()->GetItem(0));
	if (ItemIdx == idx) return;

	ItemIdx = idx;
	PlayerItem->SetSprite(ITEM_SPRITE, ItemIdx);
}