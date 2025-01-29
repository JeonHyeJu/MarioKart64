#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EngineCore/GameMode.h>

class APlayGameMode : public AGameMode
{
public:
	enum class EState
	{
		READY = 0,
		WAIT_TITLE,
		COUNT,
		PLAY,
		END,
	};

	APlayGameMode();
	~APlayGameMode();

	APlayGameMode(const APlayGameMode& _other) = delete;
	APlayGameMode(APlayGameMode&& _other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _other) noexcept = delete;

	void StartLuigiRaceway();
	void StartRoyalRaceway();
	void StartKoopaBeach();
	void StartMarioRaceway();
	void StartWarioStadium();
	void StartSherbetLand();
	void StartBowserCastle();
	void StartRainbowRoad();

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void InitEffects();

	/* Fsm start function */
	void OnGetReady();
	void OnCount();
	void OnPlay();
	void OnFinishRace();

	/* Fsm update function */
	void Readying(float _deltaTime);
	void WaitingTitle(float _deltaTime);
	void Counting(float _deltaTime);
	void Playing(float _deltaTime);
	void Finishing(float _deltaTime);

	std::shared_ptr<class ASkybox> Skybox = nullptr;
	std::shared_ptr<class ABaseMap> MapPtr = nullptr;
	//class ADriver* Player = nullptr;
	class APlayer* Player = nullptr;
	std::shared_ptr<class ALakitu> Lakitu = nullptr;

	std::shared_ptr<class ABalloons> Balloons = nullptr;
	std::shared_ptr<class ACameraActor> Camera = nullptr;

	EState State = EState::END;
	FVector CameraInitLoc = FVector{ 0.f, 100.f, -300.f };
	const float CAM_MOVE_SCALAR = 100.f;

	bool IsFinish = false;
	int ChangeCamIdx = 0;

	UFSMStateManager Fsm;
};
