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
		SelectBoxes[idx]->SetBlinkState(EBlinkState::OFF);

		if (idx == 0)
		{
			SelectBoxes[idx]->GetRectSize(&rectW, &rectH);
			SelectBoxes[idx]->SetBlinkState(EBlinkState::BLINK);
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
	else if (State == SceneState::CHANGE_GAME_TO_RULE)
	{
		ChangingGameToRule(_deltaTime);
	}
	else if (State == SceneState::CHANGE_RULE_TO_GAME)
	{
		ChangingRuleToGame(_deltaTime);
	}
	else if (State == SceneState::SELECT_RULE)
	{
		SelectingRule(_deltaTime);
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
			SelectBoxes[SelectedGameIdx]->SetBlinkState(EBlinkState::OFF);
			SelectBoxes[--SelectedGameIdx]->SetBlinkState(EBlinkState::BLINK);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		if (SelectedGameIdx < SELECT_LIST_SIZE - 1)
		{
			SelectBoxes[SelectedGameIdx]->SetBlinkState(EBlinkState::OFF);
			SelectBoxes[++SelectedGameIdx]->SetBlinkState(EBlinkState::BLINK);
		}
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		SwitchNoSelectedGameBoxes(false);

		AGameSelectBox* boxPtr = SelectBoxes[SelectedGameIdx];
		boxPtr->SetBlinkState(EBlinkState::SELECTED);
		LocOrgSelectedBox = boxPtr->GetActorLocation();
		DirMoveSelectedBox = (LocOrgSelectedBox.X < 0 ? 1 : -1);
		State = SceneState::CHANGE_GAME_TO_RULE;
	}
}

void ASelectScene::SwitchNoSelectedGameBoxes(bool _isVisible)
{
	for (int i = 0; i < SELECT_LIST_SIZE; ++i)
	{
		if (i != SelectedGameIdx)
		{
			SelectBoxes[i]->SetActive(_isVisible);
		}
	}
}

void ASelectScene::ChangingGameToRule(float _deltaTime)
{
	AGameSelectBox* boxPtr = SelectBoxes[SelectedGameIdx];
	FVector loc = boxPtr->GetActorLocation();
	if (LocOrgSelectedBox.X < 0 && loc.X > 0 || LocOrgSelectedBox.X > 0 && loc.X < 0)
	{
		boxPtr->SetBlinkStateToRuleList(0, EBlinkState::BLINK);
		State = SceneState::SELECT_RULE;
		return;
	}
	
	boxPtr->AddActorLocation({ DirMoveSelectedBox * 1000.f * _deltaTime, 0.f, 0.f });
}

void ASelectScene::ChangingRuleToGame(float _deltaTime)
{
	AGameSelectBox* boxPtr = SelectBoxes[SelectedGameIdx];
	FVector loc = boxPtr->GetActorLocation();
	if (abs(LocOrgSelectedBox.X) <= abs(loc.X))
	{
		boxPtr->SetActorLocation(LocOrgSelectedBox);
		SwitchNoSelectedGameBoxes(true);
		State = SceneState::SELECT_GAME;
		return;
	}

	boxPtr->AddActorLocation({ DirMoveSelectedBox * 1000.f * _deltaTime, 0.f, 0.f });
}

void ASelectScene::SelectingRule(float _deltaTime)
{
	AGameSelectBox* boxPtr = SelectBoxes[SelectedGameIdx];
	if (UEngineInput::IsDown(VK_UP))
	{
		boxPtr->UpBlinkStateToRuleList();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		boxPtr->DownBlinkStateToRuleList();
	}
	else if (UEngineInput::IsDown(VK_ESCAPE))
	{
		boxPtr->SetBlinkStateToRuleList(-1);
		DirMoveSelectedBox = (LocOrgSelectedBox.X < 0 ? -1 : 1);
		State = SceneState::CHANGE_RULE_TO_GAME;
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		boxPtr->SetActiveCCList(true);
		boxPtr->SetBlinkStateToRuleList(boxPtr->GetSelectedIdxRule(), EBlinkState::SELECTED);
		boxPtr->SetBlinkStateToCCList(0, EBlinkState::BLINK);
		State = SceneState::SELECT_CC;
	}
}

void ASelectScene::SelectingCC(float _deltaTime)
{
	AGameSelectBox* boxPtr = SelectBoxes[SelectedGameIdx];
	if (UEngineInput::IsDown(VK_UP))
	{
		boxPtr->UpBlinkStateToCCList();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		boxPtr->DownBlinkStateToCCList();
	}
	else if (UEngineInput::IsDown(VK_ESCAPE))
	{
		boxPtr->SetBlinkStateToCCList(-1);
		boxPtr->SetActiveCCList(false);
		State = SceneState::SELECT_RULE;
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		uint8_t idx = boxPtr->GetSelectedIdxCC();
		boxPtr->SetBlinkStateToCCList(idx, EBlinkState::OFF);
		State = SceneState::CHANGE_GAME_TO_RULE;
	}
}