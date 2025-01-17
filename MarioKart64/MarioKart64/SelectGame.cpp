#include "PreCompile.h"
#include "SelectGame.h"
#include "GameSelectBox.h"
#include "SelectButton.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

// TODO: move CC list along rule
ASelectGame::ASelectGame()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBackground = CreateDefaultSubObject<USpriteRenderer>();
	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RBtnOption = GetWorld()->SpawnActor<ASelectButton>();
	RBtnData = GetWorld()->SpawnActor<ASelectButton>();
	RBtnOk = GetWorld()->SpawnActor<ASelectButton>();

	RBackground->SetupAttachment(RootComponent);
	RTitle->SetupAttachment(RootComponent);
	RBtnOk->AttachToActor(this);
	RBtnOption->AttachToActor(this);
	RBtnData->AttachToActor(this);

	RBackground->SetSprite("SelectGame", 17);
	RBackground->SetRelativeLocation({ 0.f, 0.f, 500.f });

	InitGameSelectBox();

	RTitle->SetSprite("SelectGame", 14);
	RTitle->SetAutoScaleRatio(.75f);
	RTitle->SetRelativeLocation({ 0.f, 400.f, 0.f });

	RBtnOption->Init("SelectGame", 13, .5f);
	RBtnOption->SetActorLocation({ -625.f, -450.f, 0.f });

	RBtnData->Init("SelectGame", 12, .5f);
	RBtnData->SetActorLocation({ -350.f, -450.f, 0.f });

	RBtnOk->Init("SelectGame", 15, .5f);
	RBtnOk->SetActorLocation({ 625.f, -450.f, 0.f });

	SetIndex(0);
}

ASelectGame::~ASelectGame()
{

}

void ASelectGame::BeginPlay()
{
	AActor::BeginPlay();
}

void ASelectGame::Tick(float _deltaTime)
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
	else if (State == SceneState::WAIT_OK)
	{
		WaitingOk(_deltaTime);
	}
	else if (State == SceneState::FINISH)
	{
		OnFinish();
	}
}

void ASelectGame::InitGameSelectBox()
{
	std::vector<std::vector<SButtonData>> ruleDatas;
	ruleDatas.reserve(SELECT_LIST_SIZE);

	ruleDatas.push_back({
		SButtonData{ "SelectGame", 10, .6f, FVector{ 0.f, -240.f, 0.f }, true },
		SButtonData{ "SelectGame", 9, .6f, FVector{ 0.f, -240.f, 0.f }, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 10, .6f, FVector{ 0.f, -240.f, 0.f }, true },
		SButtonData{ "SelectGame", 11, .6f, FVector{ 0.f, -240.f, 0.f }, true },
		SButtonData{ "SelectGame", 8, .6f, FVector{ 0.f, -240.f, 0.f }, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 11, .6f, FVector{ 0.f, -240.f, 0.f }, true },
		SButtonData{ "SelectGame", 8, .6f, FVector{ 0.f, -240.f, 0.f }, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 11, .6f, FVector{ 0.f, -240.f, 0.f }, true },
		SButtonData{ "SelectGame", 8, .6f, FVector{ 0.f, -240.f, 0.f }, true },
	});

	// same
	std::vector<SButtonData> ccDatas = {
		SButtonData{ "SelectGame", 4, .6f, FVector{ 0.f, -240.f, 0.f }, false },
		SButtonData{ "SelectGame", 5, .6f, FVector{ 0.f, -240.f, 0.f }, false },
		SButtonData{ "SelectGame", 6, .6f, FVector{ 0.f, -240.f, 0.f }, false },
	};

	float rectW, rectH = 0;
	for (int i = 0, size = SELECT_LIST_SIZE; i < size; ++i)
	{
		SelectBoxes[i] = GetWorld()->SpawnActor<AGameSelectBox>().get();
		SelectBoxes[i]->Init(i);
		SelectBoxes[i]->SetBlinkState(EBlinkState::OFF);

		if (i == 0)
		{
			SelectBoxes[i]->GetRectSize(&rectW, &rectH);
			SelectBoxes[i]->SetBlinkState(EBlinkState::BLINK);

			for (SButtonData& dat : ccDatas)
			{
				dat.InitLoc += FVector{ rectW - 26.f, 5.f, 0.f };
			}
		}

		SelectBoxes[i]->AddObjectsToLayer(ELayer::RULE, ruleDatas[i]);
		SelectBoxes[i]->AddObjectsToLayer(ELayer::CC, ccDatas);

		SelectBoxes[i]->SetActorLocation({ 175.f + (i - 2) * (rectW + 30.f), 100.f, 0.f });
		SelectBoxes[i]->AttachToActor(this);
	}
}

void ASelectGame::SetIndex(int _idx)
{
	if (_idx >= 0 && _idx < SELECT_LIST_SIZE)
	{
		SelectedGameIdx = _idx;
		PtrSelectedBox = SelectBoxes[_idx];
	}
}

void ASelectGame::SelectingGame(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_LEFT))
	{
		if (SelectedGameIdx > 0)
		{
			PtrSelectedBox->SetBlinkState(EBlinkState::OFF);
			SetIndex(--SelectedGameIdx);
			PtrSelectedBox->SetBlinkState(EBlinkState::BLINK);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		if (SelectedGameIdx < SELECT_LIST_SIZE - 1)
		{
			PtrSelectedBox->SetBlinkState(EBlinkState::OFF);
			SetIndex(++SelectedGameIdx);
			PtrSelectedBox->SetBlinkState(EBlinkState::BLINK);
		}
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		SwitchNoSelectedGameBoxes(false);

		PtrSelectedBox->SetBlinkState(EBlinkState::SELECTED);
		LocOrgSelectedBox = PtrSelectedBox->GetActorLocation();
		DirMoveSelectedBox = (LocOrgSelectedBox.X < 0 ? 1 : -1);
		State = SceneState::CHANGE_GAME_TO_RULE;
	}
}

void ASelectGame::SwitchNoSelectedGameBoxes(bool _isVisible)
{
	for (int i = 0; i < SELECT_LIST_SIZE; ++i)
	{
		if (i != SelectedGameIdx)
		{
			SelectBoxes[i]->SetActive(_isVisible);
		}
	}
}

void ASelectGame::ChangingGameToRule(float _deltaTime)
{
	FVector loc = PtrSelectedBox->GetActorLocation();
	if (LocOrgSelectedBox.X < 0 && loc.X > 0 || LocOrgSelectedBox.X > 0 && loc.X < 0)
	{
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, 0, EBlinkState::BLINK);
		State = SceneState::SELECT_RULE;
		return;
	}
	
	PtrSelectedBox->AddActorLocation({ DirMoveSelectedBox * 1000.f * _deltaTime, 0.f, 0.f });
}

void ASelectGame::ChangingRuleToGame(float _deltaTime)
{
	FVector loc = PtrSelectedBox->GetActorLocation();
	if (abs(LocOrgSelectedBox.X) <= abs(loc.X))
	{
		PtrSelectedBox->SetActorLocation(LocOrgSelectedBox);
		SwitchNoSelectedGameBoxes(true);
		State = SceneState::SELECT_GAME;
		return;
	}

	PtrSelectedBox->AddActorLocation({ DirMoveSelectedBox * 1000.f * _deltaTime, 0.f, 0.f });
}

void ASelectGame::SelectingRule(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_UP))
	{
		PtrSelectedBox->MoveSelectedLayer(ELayer::RULE, EDirection::UP);
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		PtrSelectedBox->MoveSelectedLayer(ELayer::RULE, EDirection::DOWN);
	}
	else if (UEngineInput::IsDown(VK_ESCAPE))
	{
		PtrSelectedBox->SetBlinkState(EBlinkState::BLINK);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, -1);
		DirMoveSelectedBox = (LocOrgSelectedBox.X < 0 ? -1 : 1);
		State = SceneState::CHANGE_RULE_TO_GAME;
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		PtrSelectedBox->SetActiveToLayers(ELayer::CC, true);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, PtrSelectedBox->GetSelectedIdx(ELayer::RULE), EBlinkState::SELECTED);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, 0, EBlinkState::BLINK);
		State = SceneState::SELECT_CC;
	}
}

void ASelectGame::SelectingCC(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_UP))
	{
		PtrSelectedBox->MoveSelectedLayer(ELayer::CC, EDirection::UP);
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		PtrSelectedBox->MoveSelectedLayer(ELayer::CC, EDirection::DOWN);
	}
	else if (UEngineInput::IsDown(VK_ESCAPE))
	{
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, -1);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, PtrSelectedBox->GetSelectedIdx(ELayer::RULE), EBlinkState::BLINK);
		PtrSelectedBox->SetActiveToLayers(ELayer::CC, false);
		State = SceneState::SELECT_RULE;
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		uint8_t idx = PtrSelectedBox->GetSelectedIdx(ELayer::CC);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, PtrSelectedBox->GetSelectedIdx(ELayer::CC), EBlinkState::SELECTED);
		RBtnOk->SetBlinkState(EBlinkState::BLINK);
		State = SceneState::WAIT_OK;
	}
}

void ASelectGame::WaitingOk(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_ESCAPE))
	{
		RBtnOk->SetBlinkState(EBlinkState::OFF);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, PtrSelectedBox->GetSelectedIdx(ELayer::CC), EBlinkState::BLINK);
		State = SceneState::SELECT_CC;
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		State = SceneState::FINISH;
	}
}

void ASelectGame::OnFinish()
{
	State = SceneState::END;

	if (EndFuntion != nullptr)
	{
		EndFuntion();
	}
}