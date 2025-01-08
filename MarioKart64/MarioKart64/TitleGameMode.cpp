#include "PreCompile.h"
#include "TitleGameMode.h"
#include "NintendoLogo.h"
#include "Title.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

// TODO: change to fsm
ATitleGameMode::ATitleGameMode()
{
	NintendoLogo = GetWorld()->SpawnActor<ANintendoLogo>();
	Title = GetWorld()->SpawnActor<ATitle>();

	Title->SetActorLocation({ -500.0f, 500.f, 0.0f });

	NintendoLogo->Hide();
	Title->Hide();
	
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	AActor::BeginPlay();

	ShowScene(Scene::NINTENDO_LOGO);
}

void ATitleGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	if (SceneIdx < Scene::END)
	{
		CheckKey();

		if (SceneIdx == Scene::NINTENDO_LOGO)
		{
			SpinLogoAndTimeCheck(_deltaTime);
		}
	}
}

void ATitleGameMode::SpinLogoAndTimeCheck(float _deltaTime)
{
	static float eplasedSec = 0.f;
	static float angle = 60.f;

	NintendoLogo->AddActorRotation({ 0.f, angle * _deltaTime, 0.f });

	eplasedSec += _deltaTime;
	if (eplasedSec > .5f)
	{
		if (angle < 360)
		{
			angle += 10.f;
		}
		else
		{
			ShowScene(Scene::TITLE);
		}

		eplasedSec = 0.f;
	}
}

void ATitleGameMode::CheckKey()
{
	if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
	{
		int idx = static_cast<int>(SceneIdx);
		ShowScene(static_cast<Scene>(++idx));
	}
}

void ATitleGameMode::ShowScene(Scene _sceneNum)
{
	switch (_sceneNum)
	{
		case Scene::NINTENDO_LOGO:
		{
			if (SceneIdx != Scene::NINTENDO_LOGO)
			{
				NintendoLogo->Show();
				Title->Hide();
				SceneIdx = _sceneNum;
			}

			break;
		}
		case Scene::TITLE:
		{
			if (SceneIdx != Scene::TITLE)
			{
				NintendoLogo->Hide();
				Title->Show();
				SceneIdx = _sceneNum;
			}

			break;
		}
		case Scene::END:
			// TODO: Reset level
			UEngineCore::OpenLevel("PlayLevel");
			[[fallthrough]];
		default:	// IDLE
			NintendoLogo->Hide();
			Title->Hide();
			break;
	}
}

