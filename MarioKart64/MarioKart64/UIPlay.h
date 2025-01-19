#pragma once
#include <EngineCore/HUD.h>

class UImageWidget;
class AUIPlay : public AHUD
{
public:
	AUIPlay();
	~AUIPlay();

	AUIPlay(const AUIPlay& _other) = delete;
	AUIPlay(AUIPlay&& _other) noexcept = delete;
	AUIPlay& operator=(const AUIPlay& _other) = delete;
	AUIPlay& operator=(AUIPlay&& _other) noexcept = delete;

	void SetPlayerRankColor(uint8_t _val);

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

	void SetTimerUI(float _secs);
	void SetHighRankUI();
	void SetItemUI();

	std::shared_ptr<UImageWidget> Minimap = nullptr;
	std::shared_ptr<UImageWidget> PlayerRanking = nullptr;
	std::vector<UImageWidget*> MinimapLocs;

	// Lab
	std::shared_ptr<UImageWidget> LapT = nullptr;	// Title
	std::shared_ptr<UImageWidget> LapN = nullptr;	// Numerator
	std::shared_ptr<UImageWidget> LapD = nullptr;	// Denominator

	// Times
	std::shared_ptr<UImageWidget> TimeT = nullptr;
	std::shared_ptr<UImageWidget> TimeC = nullptr;
	std::shared_ptr<UImageWidget> TimeS = nullptr;
	std::vector<UImageWidget*> TimeList;

	std::vector<UImageWidget*> HighRankNumbers;
	std::vector<UImageWidget*> HighRankPlayers;

	std::shared_ptr<UImageWidget> PlayerItem = nullptr;

	const int RANK_1ST_SPRITE_IDX = 194;
	const float RANK_MUL_COLOR = 0.05f;

	const int TIME_CNT = 6;
	const int HIGH_RANK_CNT = 4;
	const int MINIMAP_CAR_INIT_IDX = 43;

	bool IsStartCount = false;
	float ElapsedSecs = 0.f;
	
	const int ZERO = 1;
	const char* RANK_PLAYER_SPRITE = "RacePositionIcons.png";
	const char* FONT_SPRITE = "FontAndPositions.png";
	const char* ITEM_SPRITE = "Items.png";

	int ItemIdx = -1;
};
