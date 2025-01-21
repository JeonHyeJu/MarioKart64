#include "PreCompile.h"
#include "SelectGame.h"
#include "GameSelectBox.h"
#include "SelectButton.h"
#include "CGlobal.h"
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

	float w = CGlobal::FWINDOW_W;
	float h = CGlobal::FWINDOW_H;
	float halfW = w * .5f;
	float halfH = h * .5f;

	RBackground->SetAutoScale(false);
	RBackground->SetScale3D({ w, h, 1.f });
	RBackground->SetSprite("SelectGame", 17);
	RBackground->SetRelativeLocation({ 0.f, 0.f, 500.f });

	InitGameSelectBox();

	RTitle->SetSprite("SelectGame", 14);
	FVector scale = RTitle->GetRealScaleOfSprite();
	RTitle->SetAutoScale(false);
	RTitle->SetScale3D({ scale.X * .5f, scale.Y * .4f, 1.f});
	RTitle->SetRelativeLocation({ 0.f, halfH * .75f, 0.f });

	const float MARGIN = 20.f;
	const float SCALE_RATIO_BTN = .35f;
	RBtnOption->Init("SelectGame", 13, SCALE_RATIO_BTN);
	scale = RBtnOption->GetRealScaleOfSprite();
	float optW = scale.X;
	RBtnOption->SetActorLocation({ -halfW + scale.X, -halfH + scale.Y, 0.f });

	RBtnData->Init("SelectGame", 12, SCALE_RATIO_BTN);
	scale = RBtnOption->GetRealScaleOfSprite();
	RBtnData->SetActorLocation({ -halfW + optW + scale.X + MARGIN, -halfH + scale.Y, 0.f });

	RBtnOk->Init("SelectGame", 15, SCALE_RATIO_BTN);
	scale = RBtnOk->GetRealScaleOfSprite();
	RBtnOk->SetActorLocation({ halfW - scale.X, -halfH + scale.Y, 0.f });
	RBtnOk->SetActive(false);

	SetIndex(0);

	Fsm.CreateState(ESceneState::SELECT_GAME, std::bind(&ASelectGame::SelectingGame, this, std::placeholders::_1));
	Fsm.CreateState(ESceneState::CHANGE_GAME_TO_RULE, std::bind(&ASelectGame::ChangingGameToRule, this, std::placeholders::_1));
	Fsm.CreateState(ESceneState::CHANGE_RULE_TO_GAME, std::bind(&ASelectGame::ChangingRuleToGame, this, std::placeholders::_1));
	Fsm.CreateState(ESceneState::SELECT_RULE, std::bind(&ASelectGame::SelectingRule, this, std::placeholders::_1));
	Fsm.CreateState(ESceneState::SELECT_CC, std::bind(&ASelectGame::SelectingCC, this, std::placeholders::_1), std::bind(&ASelectGame::OnSelectCC, this));
	Fsm.CreateState(ESceneState::WAIT_OK, std::bind(&ASelectGame::WaitingOk, this, std::placeholders::_1), std::bind(&ASelectGame::OnWaitOk, this));
	Fsm.CreateState(ESceneState::FINISH, nullptr, std::bind(&ASelectGame::OnFinish, this));
	Fsm.CreateState(ESceneState::END, nullptr);
}

ASelectGame::~ASelectGame()
{

}

void ASelectGame::BeginPlay()
{
	AActor::BeginPlay();

	Fsm.ChangeState(ESceneState::SELECT_GAME);
}

void ASelectGame::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void ASelectGame::InitGameSelectBox()
{
	std::vector<std::vector<SButtonData>> ruleDatas;
	ruleDatas.reserve(SELECT_LIST_SIZE);
	
	const float SCALE_RATIO = .45f;
	const FVector INIT_LOC{ 0.f, -190.f, 0.f };

	ruleDatas.push_back({
		SButtonData{ "SelectGame", 10, SCALE_RATIO, INIT_LOC, true },
		SButtonData{ "SelectGame", 9, SCALE_RATIO, INIT_LOC, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 10, SCALE_RATIO, INIT_LOC, true },
		SButtonData{ "SelectGame", 11, SCALE_RATIO, INIT_LOC, true },
		SButtonData{ "SelectGame", 8, SCALE_RATIO, INIT_LOC, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 11, SCALE_RATIO, INIT_LOC, true },
		SButtonData{ "SelectGame", 8, SCALE_RATIO, INIT_LOC, true },
	});
	ruleDatas.push_back({
		SButtonData{ "SelectGame", 11, SCALE_RATIO, INIT_LOC, true },
		SButtonData{ "SelectGame", 8, SCALE_RATIO, INIT_LOC, true },
	});

	// same
	std::vector<SButtonData> ccDatas = {
		SButtonData{ "SelectGame", 4, SCALE_RATIO, INIT_LOC, false },
		SButtonData{ "SelectGame", 5, SCALE_RATIO, INIT_LOC, false },
		SButtonData{ "SelectGame", 6, SCALE_RATIO, INIT_LOC, false },
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

/* Fsm start function */
void ASelectGame::OnSelectCC()
{
	RBtnOk->SetActive(false);
}

void ASelectGame::OnWaitOk()
{
	RBtnOk->SetActive(true);
	RBtnOk->SetActorRotation({ 0.f, 90.f, 0.f });
}

void ASelectGame::OnFinish()
{
	if (EndFuntion != nullptr)
	{
		EndFuntion();
	}
}

/* Fsm update function */
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
		Fsm.ChangeState(ESceneState::CHANGE_GAME_TO_RULE);
	}
}

void ASelectGame::ChangingGameToRule(float _deltaTime)
{
	FVector loc = PtrSelectedBox->GetActorLocation();
	if (LocOrgSelectedBox.X < 0 && loc.X > 0 || LocOrgSelectedBox.X > 0 && loc.X < 0)
	{
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, 0, EBlinkState::BLINK);
		Fsm.ChangeState(ESceneState::SELECT_RULE);
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
		Fsm.ChangeState(ESceneState::SELECT_GAME);
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
		Fsm.ChangeState(ESceneState::CHANGE_RULE_TO_GAME);
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		PtrSelectedBox->SetActiveToLayers(ELayer::CC, true);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::RULE, PtrSelectedBox->GetSelectedIdx(ELayer::RULE), EBlinkState::SELECTED);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, 0, EBlinkState::BLINK);
		Fsm.ChangeState(ESceneState::SELECT_CC);
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
		Fsm.ChangeState(ESceneState::SELECT_RULE);
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		uint8_t idx = PtrSelectedBox->GetSelectedIdx(ELayer::CC);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, PtrSelectedBox->GetSelectedIdx(ELayer::CC), EBlinkState::SELECTED);
		RBtnOk->SetBlinkState(EBlinkState::BLINK);
		Fsm.ChangeState(ESceneState::WAIT_OK);
	}
}

void ASelectGame::WaitingOk(float _deltaTime)
{
	float rotY = RBtnOk->GetTransform().Rotation.Y;
	if (rotY > 0)
	{
		RBtnOk->AddActorRotation({ 0.f, -180.f * _deltaTime, 0.f });
	}

	if (UEngineInput::IsDown(VK_ESCAPE))
	{
		RBtnOk->SetBlinkState(EBlinkState::OFF);
		PtrSelectedBox->SetBlinkStateToLayer(ELayer::CC, PtrSelectedBox->GetSelectedIdx(ELayer::CC), EBlinkState::BLINK);
		Fsm.ChangeState(ESceneState::SELECT_CC);
	}
	else if (UEngineInput::IsDown(VK_RETURN) || UEngineInput::IsDown(VK_SPACE))
	{
		Fsm.ChangeState(ESceneState::FINISH);
	}
}