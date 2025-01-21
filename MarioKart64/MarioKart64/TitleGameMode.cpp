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
	Fsm.CreateState(EScene::END, nullptr, std::bind(&ATitleGameMode::OnEnd, this));
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

	GetWorld()->GetMainCamera()->GetCameraComponent()->SetProjectionType(EProjectionType::Perspective);
}

void ATitleGameMode::OnShowTitle()
{
	NintendoLogo->SetActive(false);
	Title->SetActive(true);

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
	if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
	{
		Fsm.ChangeState(EScene::END);
	}
}