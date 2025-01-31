#pragma once
#include <EngineCore/HUD.h>
#include <EngineBase/FSMStateManager.h>

class UImageWidget;
class WTextWrapper;
class AUIPlay : public AHUD
{
public:
	enum class EState
	{
		IDLE = 0,
		READY,
		WAIT_PLAY_COUNT,
		PLAY_RACING,
		FINISH_RACING,
		RESULT,
		RESULT_2,
		WAIT_PLAY_FX,
		TOTAL,
		TOTAL_2,
		END
	};

	AUIPlay();
	~AUIPlay();

	AUIPlay(const AUIPlay& _other) = delete;
	AUIPlay(AUIPlay&& _other) noexcept = delete;
	AUIPlay& operator=(const AUIPlay& _other) = delete;
	AUIPlay& operator=(AUIPlay&& _other) noexcept = delete;

	void SetPlayerRankColor(uint8_t _val);
	void Reset();

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void InitMinimap();
	void InitPlayerRank();
	void InitElpasedTime();
	void InitHighRank();
	void InitItem();
	void InitMinimapLoc();
	void InitLap();
	void InitTexts();
	void InitLetterBox();
	void InitTitle();

	void ResetTimer();
	void StartTimer()
	{
		IsStartCount = true;
	}
	void StopTimer()
	{
		IsStartCount = false;
		ResetTimer();
	}

	void CountTimer(float _deltaTime);
	bool ShowTexts(float _deltaTime);

	void SetTimerUI(float _secs);
	void SetHighRankUI();
	void SetItemUI();
	void SetMinimapLoc();

	void SetPlayUIVisible(bool _val);

	void TickLap(float _deltaTime);

	void AnimPlayerRankColor(float _deltaTime);

	/* Fsm start function */
	void OnIdle();
	void OnOpenRacing();
	void OnWaitCount();
	void OnRacing();
	void OnFinishRace();
	void OnShowResult();
	void OnShowResult2();
	void OnWait();
	void OnShowTotal();

	/* Fsm update function */
	void Idleing(float _deltaTime);
	void OpeningRacing(float _deltaTime);
	void WaitingCount(float _deltaTime);
	void Racing(float _deltaTime);
	void FinishingRace(float _deltaTime);
	void ShowingResult(float _deltaTime);
	void ShowingResult2(float _deltaTime);
	void Waiting(float _deltaTime);
	void ShowingTotal(float _deltaTime);

	static const int HIGH_RANK_CNT = 6;

	std::shared_ptr<UImageWidget> Minimap = nullptr;
	std::vector<UImageWidget*> MinimapLocs;

	// Player ranking
	std::shared_ptr<UImageWidget> PlayerRanking = nullptr;
	FVector RankingScale = FVector::ZERO;
	FVector RankingScaleBig = FVector::ZERO;

	// Lab
	std::shared_ptr<UImageWidget> LapT = nullptr;	// Title
	std::shared_ptr<UImageWidget> LapC = nullptr;	// Count

	// Times
	std::shared_ptr<UImageWidget> TimeT = nullptr;
	std::shared_ptr<UImageWidget> TimeC = nullptr;
	std::shared_ptr<UImageWidget> TimeS = nullptr;
	std::vector<UImageWidget*> TimeList;

	// High rank
	std::vector<UImageWidget*> HighRankNumbers;
	std::vector<UImageWidget*> HighRankPlayers;
	FVector HighRankLocs[HIGH_RANK_CNT];
	FVector HighNumLocs[HIGH_RANK_CNT];
	float HeightRankImg = 0.f;

	// Player's itembox
	std::shared_ptr<UImageWidget> PlayerItem = nullptr;

	// Letter box
	std::shared_ptr<class WLetterboxWidget> LetterBox = nullptr;

	// Racing title
	std::shared_ptr<WTextWrapper> TitleOfCup = nullptr;
	std::shared_ptr<WTextWrapper> TitleOfMap = nullptr;
	FVector InitLocTitleCup = { -280.f, 330.f };
	FVector InitLocTitleMap = { -270.f, -330.f };

	// Result texts
	std::shared_ptr<WTextWrapper> TextWrapperU = nullptr;
	std::shared_ptr<WTextWrapper> TextWrapperL = nullptr;
	const float InitX_U = 650.f;
	const float MoveX_U = 50.f;
	const float InitX_L = -1200.f ;
	const float MoveX_L = -550.f;

	// Temp
	FVector MinimapSizeInfo = FVector::ZERO;

	const int RANK_HIGH_SPRITE_IDX = 58;
	const int RANK_1ST_SPRITE_IDX = 67;
	const float RANK_MUL_COLOR = 0.05f;

	const int TIME_CNT = 6;
	const int MINIMAP_CAR_INIT_IDX = 43;

	bool IsStartCount = false;
	float ElapsedSecs = 0.f;
	
	const int ZERO = 1;
	const char* RANK_PLAYER_SPRITE = "RacePositionIcons.png";
	const char* FONT_SPRITE = "FontAndPositions.png";
	const char* ITEM_SPRITE = "Items.png";
	const char* LAP_SPRITE = "LabCount";

	int ItemIdx = -1;
	int ShowingLap = 1;

	float CurH = 150.f;

	UFSMStateManager Fsm;
};
