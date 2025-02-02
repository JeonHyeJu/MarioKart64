#pragma once
#include <EngineCore/GameMode.h>
#include <EngineBase/FSMStateManager.h>
#include <EnginePlatform/EngineSound.h>

class ASelectGameMode : public AGameMode
{
public:
	enum class Scene
	{
		IDLE = 0,
		SELECT_GAME,
		SELECT_CHARACTER,
		SELECT_MAP,
		END,
	};

	ASelectGameMode();
	~ASelectGameMode();

	ASelectGameMode(const ASelectGameMode& _other) = delete;
	ASelectGameMode(ASelectGameMode&& _other) noexcept = delete;
	ASelectGameMode& operator=(const ASelectGameMode& _other) = delete;
	ASelectGameMode& operator=(ASelectGameMode&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;
	void LevelChangeEnd() override;

private:
	/* Fsm start function */
	void OnShowSelectGame();
	void OnShowSelectCharacter();
	void OnShowSelectMap();

	/* Callbacks */
	void OnEndSelectGame();
	void OnEndSelectCharacter();
	void OnEndSelectMap();

	std::shared_ptr<class ASelectGame> SelectGame = nullptr;
	std::shared_ptr<class ASelectCharacter> SelectCharacter = nullptr;
	std::shared_ptr<class ASelectMap> SelectMap = nullptr;
	//std::shared_ptr<class AFade> Fade = nullptr;	// TODO

	UFSMStateManager Fsm;

	/* Sounds */
	USoundPlayer BgmSP;
};
