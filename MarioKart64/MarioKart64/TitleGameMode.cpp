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

	ShowScene(SceneNum::NINTENDO_LOGO);
}

void ATitleGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	static float eplasedSec = 0.f;
	static float angle = 60.f;

	CheckKey();

	if (SceneIdx == SceneNum::NINTENDO_LOGO)
	{
		NintendoLogo->AddActorRotation({ 0.f, angle * _deltaTime, 0.f });
	}

	eplasedSec += _deltaTime;
	if (eplasedSec > .5f)
	{
		if (angle < 360)
		{
			angle += 10.f;
		}
		else
		{
			ShowScene(SceneNum::TITLE);
		}

		eplasedSec = 0.f;
	}
}

void ATitleGameMode::CheckKey()
{
	if (UEngineInput::IsDown(VK_SPACE) || UEngineInput::IsDown(VK_RETURN))
	{
		if (SceneIdx < SceneNum::END)
		{
			int idx = static_cast<int>(SceneIdx);
			ShowScene(static_cast<SceneNum>(++idx));
		}
	}
}

void ATitleGameMode::ShowScene(SceneNum _sceneNum)
{
	switch (_sceneNum)
	{
		case SceneNum::NINTENDO_LOGO:
		{
			if (SceneIdx != SceneNum::NINTENDO_LOGO)
			{
				NintendoLogo->Show();
				Title->Hide();
				SceneIdx = _sceneNum;
			}
			break;
		}
		case SceneNum::TITLE:
		{
			if (SceneIdx != SceneNum::TITLE)
			{
				NintendoLogo->Hide();
				Title->Show();
				SceneIdx = _sceneNum;
			}
			break;
		}
		case SceneNum::END:
			NintendoLogo->Hide();
			Title->Hide();

			// TODO: Reset level
			UEngineCore::OpenLevel("PlayLevel");
			break;
		default:	// IDLE
			break;
	}
}

