#include "PreCompile.h"
#include "SelectMap.h"
#include "SelectButton.h"
#include "MapSpec.h"
#include "CGlobal.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ASelectMap::ASelectMap()
{
	const char* SPRITE_NAME = "TrackIcons.png";

	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	float w = CGlobal::FWINDOW_W;
	float h = CGlobal::FWINDOW_H;
	float halfW = w * .5f;
	float halfH = h * .5f;

	RBg = CreateDefaultSubObject<USpriteRenderer>();
	RBg->SetSprite("Background", 2);
	RBg->SetAutoScale(false);
	RBg->SetScale3D({ w, h, 1.f });
	RBg->SetRelativeLocation({ 0.f, 0.f, 500.f });
	RBg->SetupAttachment(RootComponent);
	
	const float SCALE_RATIO = 4.f;
	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RTitle->SetSprite(SPRITE_NAME, 0);
	FVector scale = RTitle->GetRealScaleOfSprite();
	RTitle->SetAutoScaleRatio(SCALE_RATIO);
	RTitle->SetRelativeLocation({ 0.f, halfH * .7f, -0.f });
	RTitle->SetupAttachment(RootComponent);

	BtnOk = GetWorld()->SpawnActor<ASelectButton>();
	BtnOk->Init("SelectGame", 15, .4f);
	scale = BtnOk->GetRealScaleOfSprite();
	BtnOk->AttachToActor(this);
	BtnOk->SetActorLocation({ halfW - scale.X, -halfH + scale.Y, 0.f });
	BtnOk->SetActive(false);

	{
		int idxs[4] = { 2, 21, 39, 57 };
		float width = -1.f;
		float height = 100.f;
		const float INIT_X = -380.f;
		const float MARGIN = 20.f;
		for (int i = 0; i < SIZE; ++i)
		{
			std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
			ptr->Init(SPRITE_NAME, idxs[i], SCALE_RATIO);

			if (width < 0)
			{
				FVector scale = ptr->GetRealScaleOfSprite();
				width = scale.X;
			}

			ptr->SetActorLocation({ INIT_X + i * (width + MARGIN), height, -5.f });
			ptr->AttachToActor(this);
			MainRects[i] = ptr.get();
		}
	}

	MapSpec = GetWorld()->SpawnActor<AMapSpec>();
	MapSpec->SetActorLocation({ 100.f, 100.f, -5.f });
	MapSpec->AttachToActor(this);

	Fsm.CreateState(ESceneState::SELECT_MAP, std::bind(&ASelectMap::Selecting, this, std::placeholders::_1), std::bind(&ASelectMap::OnShowSelectMap, this));
	Fsm.CreateState(ESceneState::WAIT_OK, std::bind(&ASelectMap::WaitingOk, this, std::placeholders::_1), std::bind(&ASelectMap::OnWaitOk, this));
	Fsm.CreateState(ESceneState::FINISH, nullptr, std::bind(&ASelectMap::OnFinish, this));
	Fsm.CreateState(ESceneState::END, nullptr);

	Hover(CurSelectedIdx);
}

ASelectMap::~ASelectMap()
{

}

void ASelectMap::BeginPlay()
{
	AActor::BeginPlay();

	Fsm.ChangeState(ESceneState::SELECT_MAP);
}

void ASelectMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	Fsm.Update(_deltaTime);
}

void ASelectMap::Hover(uint8_t _idx)
{
	if (_idx >= SIZE) return;

	PreSelectedIdx = CurSelectedIdx;
	CurSelectedIdx = _idx;

	MainRects[PreSelectedIdx]->SetBlinkState(EBlinkState::OFF);
	MainRects[_idx]->SetBlinkState(EBlinkState::BLINK);
	MapSpec->Init(_idx);
}

/* Fsm start function */
void ASelectMap::OnShowSelectMap()
{
	BtnOk->SetBlinkState(EBlinkState::OFF);
	BtnOk->SetActive(false);
}

void ASelectMap::OnWaitOk()
{
	BtnOk->SetActive(true);
	BtnOk->SetBlinkState(EBlinkState::BLINK);
	BtnOk->SetActorRotation({ 0.f, 90.f, 0.f });
}

void ASelectMap::OnFinish()
{
	BtnOk->SetBlinkState(EBlinkState::SELECTED);
	Fsm.ChangeState(ESceneState::END);

	if (EndFuntion != nullptr)
	{
		EndFuntion();
	}
}

/* Fsm update function */
void ASelectMap::Selecting(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_LEFT))
	{
		if (CurSelectedIdx > 0)
		{
			Hover(CurSelectedIdx - 1);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		// manually block
		return;

		if (CurSelectedIdx < SIZE - 1)
		{
			Hover(CurSelectedIdx + 1);
		}
	}
	else if (UEngineInput::IsDown(VK_SPACE))
	{
		Fsm.ChangeState(ESceneState::WAIT_OK);
	}
}

void ASelectMap::WaitingOk(float _deltaTime)
{
	float rotY = BtnOk->GetTransform().Rotation.Y;
	if (rotY > 0)
	{
		BtnOk->AddActorRotation({ 0.f, -180.f * _deltaTime, 0.f });
	}

	if (UEngineInput::IsDown(VK_SPACE))
	{
		Fsm.ChangeState(ESceneState::FINISH);
	}
	else if (UEngineInput::IsDown(VK_ESCAPE))
	{
		Fsm.ChangeState(ESceneState::SELECT_MAP);
	}
}