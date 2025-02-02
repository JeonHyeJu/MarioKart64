#include "PreCompile.h"
#include "TitleGameMode.h"
#include "NintendoLogo.h"
#include "Title.h"
#include "CGlobal.h"
#include <EngineCore/EngineCamera.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

ATitleGameMode::ATitleGameMode()
{
	NintendoLogo = GetWorld()->SpawnActor<ANintendoLogo>();
	Title = GetWorld()->SpawnActor<ATitle>();

	int winH = CGlobal::WINDOW_H;
	NintendoLogo->SetActorLocation({ 0.f, -(winH * .15f), 0.f });
	Title->SetActorLocation({ 0.0f, 0.f, 0.0f });

	NintendoLogo->SetActive(false);
	Title->SetActive(false);
	
	GetWorld()->GetMainCamera()->SetActorLocation({ 0.0f, 0.0f, -1000.0f });

	Fsm.CreateState(EScene::NINTENDO_LOGO, std::bind(&ATitleGameMode::ShowingLogo, this, std::placeholders::_1), std::bind(&ATitleGameMode::OnShowLogo, this));
	Fsm.CreateState(EScene::TITLE, std::bind(&ATitleGameMode::ShowingTitle, this, std::placeholders::_1), std::bind(&ATitleGameMode::OnShowTitle, this));
	Fsm.CreateState(EScene::WAIT_SOUND, std::bind(&ATitleGameMode::WaitingSound, this, std::placeholders::_1));
	Fsm.CreateState(EScene::END, std::bind(&ATitleGameMode::OnEnd, this));
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	AActor::BeginPlay();

	Fsm.ChangeState(EScene::NINTENDO_LOGO);
}

void ATitleGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void ATitleGameMode::LevelChangeEnd()
{
	BgmSP.Stop();
}

void ATitleGameMode::SpinLogoAndTimeCheck(float _deltaTime)
{
	NintendoLogo->AddActorRotation({ 0.f, LogoAngle * _deltaTime, 0.f });

	LogoElapsedSecs += _deltaTime;
	if (LogoElapsedSecs > .5f)
	{
		if (LogoAngle < 720)
		{
			LogoAngle += 45.f;
		}
		else
		{
			Fsm.ChangeState(EScene::TITLE);
		}

		LogoElapsedSecs = 0.f;
	}
}

/* Fsm start function */
void ATitleGameMode::OnShowLogo()
{
	NintendoLogo->SetActive(true);
	Title->SetActive(false);

	LogoElapsedSecs = 0.f;
	LogoAngle = 60.f;

	IntroSP = UEngineSound::Play("Intro.wav");
	IntroSP.SetVolume(.4f);

	GetWorld()->GetMainCamera()->GetCameraComponent()->SetProjectionType(EProjectionType::Perspective);
}

void ATitleGameMode::OnShowTitle()
{
	NintendoLogo->SetActive(false);
	Title->SetActive(true);

	IntroSP.Stop();
	BgmSP = UEngineSound::Play("01.OpeningTitle.mp3");
	BgmSP.SetVolume(.4f);
	
	GetWorld()->GetMainCamera()->GetCameraComponent()->SetProjectionType(EProjectionType::Orthographic);
}

void ATitleGameMode::OnEnd()
{
	UEngineCore::OpenLevel("SelectLevel");
}

/* Fsm update function */
void ATitleGameMode::ShowingLogo(float _deltaTime)
{
	SpinLogoAndTimeCheck(_deltaTime);

	if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
	{
		Fsm.ChangeState(EScene::TITLE);
	}
}

void ATitleGameMode::ShowingTitle(float _deltaTime)
{
	static bool isVoiceLaunched = false;
	static float elpasedSecs = 0.f;

	if (isVoiceLaunched == false)
	{
		elpasedSecs += _deltaTime;
		if (elpasedSecs > 1.f)
		{
			isVoiceLaunched = true;
			elpasedSecs = 0.f;

			USoundPlayer welcomePlayer = UEngineSound::Play("Welcome.wav");
			welcomePlayer.SetVolume(.25f);
		}
	}

	if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
	{
		StartSP = UEngineSound::Play("StartGame.mp3");
		StartSP.SetVolume(.4f);

		Fsm.ChangeState(EScene::WAIT_SOUND);
	}
}

void ATitleGameMode::WaitingSound(float _deltaTime)
{
	if (StartSP.IsPlaying() == false)
	{
		Fsm.ChangeState(EScene::END);
	}
}