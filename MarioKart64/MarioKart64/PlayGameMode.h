#pragma once
#include <EngineCore/GameMode.h>

class APlayGameMode : public AGameMode
{
public:
	enum class EState
	{
		START = 0,
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

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void Starting(float _deltaTime);
	void Playing(float _deltaTime);

	std::shared_ptr<class ASkybox> Skybox = nullptr;
	std::shared_ptr<class ABaseMap> MapPtr = nullptr;
	//class ADriver* Player = nullptr;
	class APlayer* Player = nullptr;

	std::shared_ptr<class ABalloons> Balloons = nullptr;

	EState State = EState::END;
	FVector CameraInitLoc = FVector{ 0.f, 100.f, -300.f };
	FVector CameraMoveLoc = FVector{ 0.f, 1.f, -1.f };
	const float CAM_MOVE_SCALAR = 100.f;
};
