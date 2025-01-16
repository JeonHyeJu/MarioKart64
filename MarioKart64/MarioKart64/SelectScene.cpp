#include "PreCompile.h"
#include "SelectScene.h"
#include "GameSelectBox.h"
#include "SelectButton.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

ASelectScene::ASelectScene()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBackground = CreateDefaultSubObject<USpriteRenderer>();
	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RBtnOption = GetWorld()->SpawnActor<ASelectButton>();
	RBtnData = GetWorld()->SpawnActor<ASelectButton>();

	RBackground->SetupAttachment(RootComponent);
	RTitle->SetupAttachment(RootComponent);
	RBtnOption->AttachToActor(this);
	RBtnData->AttachToActor(this);

	RBackground->SetSprite("SelectGame", 17);
	RBackground->SetRelativeLocation({ 0.f, 0.f, 500.f });

	float rectW, rectH = 0;
	for (int i = -2; i < SELECT_LIST_SIZE-2; ++i)
	{
		int idx = i + 2;
		SelectBoxes[idx] = GetWorld()->SpawnActor<AGameSelectBox>().get();
		SelectBoxes[idx]->Init(idx);
		SelectBoxes[idx]->SetSelected(false);

		if (idx == 0)
		{
			SelectBoxes[idx]->GetRectSize(&rectW, &rectH);
			SelectBoxes[idx]->SetSelected(true);
		}

		SelectBoxes[idx]->SetActorLocation({ 175.f + i * (rectW + 30.f), 100.f, 0.f });
		SelectBoxes[idx]->AttachToActor(this);
	}
	
	RTitle->SetSprite("SelectGame", 14);
	RTitle->SetAutoScaleRatio(.75f);
	RTitle->SetRelativeLocation({ 0.f, 400.f, 0.f });

	RBtnOption->Init("SelectGame", 13, .5f);
	RBtnOption->SetActorLocation({ -625.f, -450.f, 0.f });

	RBtnData->Init("SelectGame", 12, .5f);
	RBtnData->SetActorLocation({ -350.f, -450.f, 0.f });
}

ASelectScene::~ASelectScene()
{

}

void ASelectScene::BeginPlay()
{
	AActor::BeginPlay();
}

void ASelectScene::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	// Change to fsm
	if (State == SceneState::SELECT_GAME)
	{
		SelectingGame(_deltaTime);
	}
	else if (State == SceneState::CHANGE_GAME_TO_CC)
	{
		ChangingGameToCC(_deltaTime);
	}
	else if (State == SceneState::SELECT_CC)
	{
		SelectingCC(_deltaTime);
	}
}

void ASelectScene::SelectingGame(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_LEFT))
	{
		if (SelectedGameIdx > 0)
		{
			SelectBoxes[SelectedGameIdx]->SetSelected(false);
			SelectBoxes[--SelectedGameIdx]->SetSelected(true);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		if (SelectedGameIdx < SELECT_LIST_SIZE - 1)
		{
			SelectBoxes[SelectedGameIdx]->SetSelected(false);
			SelectBoxes[++SelectedGameIdx]->SetSelected(true);
		}
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		for (int i = 0; i < SELECT_LIST_SIZE; ++i)
		{
			if (i != SelectedGameIdx)
			{
				SelectBoxes[i]->SetActive(false);
			}
		}
	}
}

void ASelectScene::ChangingGameToCC(float _deltaTime)
{

}

void ASelectScene::SelectingCC(float _deltaTime)
{

}