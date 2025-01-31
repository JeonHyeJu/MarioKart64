#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EngineCore/GameMode.h>

class APlayGameMode : public AGameMode
{
public:
	enum class EState
	{
		READY = 0,
		WAIT_UI_OPEN,
		COUNT,
		PLAY,
		FINISH,
		WAIT_UI_RESULT,
		WAIT_KEY,
		END,
	};

	APlayGameMode();
	~APlayGameMode();

	APlayGameMode(const APlayGameMode& _other) = delete;
	APlayGameMode(APlayGameMode&& _other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void InitCharacters();
	void InitEffects();
	void InitMap();
	void SetCamFinishRot();

	/* Fsm start function */
	void OnGetReady();
	void OnCount();
	void OnPlay();
	void OnFinish();
	void OnWaitUIResult();
	void OnWaitKey();

	/* Fsm update function */
	void Readying(float _deltaTime);
	void WaitingUIOpen(float _deltaTime);
	void Counting(float _deltaTime);
	void Playing(float _deltaTime);
	void Finishing(float _deltaTime);
	void WaitingUIResult(float _deltaTime);
	void WaitingKey(float _deltaTime);

	std::shared_ptr<class ASkybox> Skybox = nullptr;
	std::shared_ptr<class ABaseMap> MapPtr = nullptr;
	std::shared_ptr<class ALakitu> Lakitu = nullptr;
	class APlayer* Player = nullptr;
	class ADriver* Players[6] = { nullptr, };

	std::shared_ptr<class ABalloons> Balloons = nullptr;
	std::shared_ptr<class ACameraActor> Camera = nullptr;

	EState State = EState::END;
	FVector CameraInitLoc = FVector{ 0.f, 100.f, -300.f };
	const float CAM_MOVE_SCALAR = 100.f;

	UFSMStateManager Fsm;
};
