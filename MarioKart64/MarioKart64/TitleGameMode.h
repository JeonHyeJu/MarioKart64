#pragma once
#include <EngineCore/GameMode.h>
#include <EngineBase/FSMStateManager.h>
#include <EnginePlatform/EngineSound.h>

class ATitleGameMode : public AGameMode
{
public:
	enum class EScene
	{
		IDLE = 0,
		NINTENDO_LOGO,
		TITLE,
		WAIT_SOUND,
		END,
	};

	ATitleGameMode();
	~ATitleGameMode();

	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void LevelChangeEnd() override;

private:
	void SpinLogoAndTimeCheck(float _deltaTime);

	/* Fsm start function */
	void OnShowLogo();
	void OnShowTitle();
	void OnEnd();

	/* Fsm update function */
	void ShowingLogo(float _deltaTime);
	void ShowingTitle(float _deltaTime);
	void WaitingSound(float _deltaTime);

	std::shared_ptr<class ANintendoLogo> NintendoLogo = nullptr;
	std::shared_ptr<class ATitle> Title = nullptr;

	UFSMStateManager Fsm;

	float LogoElapsedSecs = 0.f;
	float LogoAngle = 60.f;

	/* Sounds */
	USoundPlayer StartSP;
	USoundPlayer IntroSP;
	USoundPlayer BgmSP;
};