#include "PreCompile.h"
#include "SelectCharacter.h"
#include "SelectButton.h"
#include "CharacterAndName.h"
#include "CGlobal.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ASelectCharacter::ASelectCharacter()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	float w = CGlobal::FWINDOW_W;
	float h = CGlobal::FWINDOW_H;
	float halfW = w * .5f;
	float halfH = h * .5f;

	RBg = CreateDefaultSubObject<USpriteRenderer>();
	RBg->SetSprite("Background", 1);
	RBg->SetAutoScale(false);
	RBg->SetScale3D({ w, h, 1.f });
	RBg->SetRelativeLocation({ 0.f, 0.f, 500.f });
	RBg->SetupAttachment(RootComponent);

	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RTitle->SetSprite("SelectCharacter", 0);
	FVector scale = RTitle->GetRealScaleOfSprite();
	RTitle->SetAutoScale(false);
	RTitle->SetScale3D({ scale.X * .5f, scale.Y * .4f, 1.f });
	RTitle->SetRelativeLocation({ 0.f, halfH * .75f, 0.f });
	RTitle->SetupAttachment(RootComponent);

	BtnOk = GetWorld()->SpawnActor<ASelectButton>();
	BtnOk->Init("SelectGame", 15, .6f);
	scale = BtnOk->GetRealScaleOfSprite();
	BtnOk->AttachToActor(this);
	BtnOk->SetActorLocation({ halfW - scale.X, -halfH + scale.Y, 0.f });
	BtnOk->SetActive(false);

	InitCharacterImgs();
	scale = ImageList[0]->GetScale();
	FVector scaleName = ImageList[0]->GetScaleName();

	RSelect = CreateDefaultSubObject<USpriteRenderer>();
	RSelect->SetSprite("SelectCharacter.png", 164);
	RSelect->SetAutoScale(false);
	RSelect->SetScale3D(scale - FVector{ 0.f, scaleName.Y * .5f, 0.f });
	RSelect->SetupAttachment(RootComponent);

	RPlayer1 = CreateDefaultSubObject<USpriteRenderer>();
	RPlayer1->SetSprite("SelectCharacter.png", 160);
	RPlayer1->SetAutoScale(false);
	RPlayer1->SetScale3D(scale - FVector{ 0.f, scaleName.Y * .5f, 0.f });
	RPlayer1->SetupAttachment(RootComponent);

	SelectMargin = FVector{ 0.f, scaleName.Y * .125f, -1.f };

	Fsm.CreateState(ESceneState::SELECT, std::bind(&ASelectCharacter::Selecting, this, std::placeholders::_1), std::bind(&ASelectCharacter::OnSelect, this));
	Fsm.CreateState(ESceneState::SELECT_MOVING, std::bind(&ASelectCharacter::Moving, this, std::placeholders::_1), std::bind(&ASelectCharacter::OnSelectMoveForward, this));
	Fsm.CreateState(ESceneState::SELECT_MOVING_REVERSE, std::bind(&ASelectCharacter::Moving, this, std::placeholders::_1), std::bind(&ASelectCharacter::OnSelectMoveBackward, this));
	Fsm.CreateState(ESceneState::WAIT_OK, std::bind(&ASelectCharacter::Waiting, this, std::placeholders::_1), std::bind(&ASelectCharacter::OnWaitOk, this));
	Fsm.CreateState(ESceneState::FINISH, nullptr, std::bind(&ASelectCharacter::OnFinish, this));
	Fsm.CreateState(ESceneState::END, nullptr);
}

ASelectCharacter::~ASelectCharacter()
{

}

void ASelectCharacter::BeginPlay()
{
	AActor::BeginPlay();
	srand(static_cast<unsigned int>(time(nullptr)));

	MoveSelectUI(0);
	Fsm.ChangeState(ESceneState::SELECT);
}

void ASelectCharacter::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
	Fsm.Update(_deltaTime);
}

void ASelectCharacter::InitCharacterImgs()
{
	float w = CGlobal::FWINDOW_W;
	float h = CGlobal::FWINDOW_H;
	float halfW = w * .5f;
	
	const float MARGIN_X = 30.f;
	const float INITIAL_X = -halfW * .685f;
	const float INITIAL_Y = 100.f;
	
	const float SCALE_RATIO = 4.f;
	const char* SPRITE_NAME = "SelectCharacter.png";

	ULevel* pLevel = GetWorld();
	// The order is the same with ECharacter
	for (int i = 0; i < SIZE; ++i)
	{
		std::shared_ptr<ACharacterAndName> ptr = pLevel->SpawnActor<ACharacterAndName>();
		ptr->Init(SPRITE_NAME, i, SCALE_RATIO);
		FVector scale = ptr->GetScale();

		float x = INITIAL_X + (scale.X + MARGIN_X) * (i % 4);
		float y = INITIAL_Y + ((i < 4) ? 0.f : -scale.Y - MARGIN_Y);
		FVector loc{ x, y, -10.f };

		ptr->SetActorLocation(loc);
		ptr->AttachToActor(this);

		for (int j = 0, size = ARRAYSIZE(DISABLE_IDXS); j < size; ++j)
		{
			if (i == DISABLE_IDXS[j])
			{
				ptr->SetEnable(false);
				break;
			}
		}

		ImageList[i] = ptr;
	}
}

void ASelectCharacter::RunBlink(int _idx)
{
	ImageList[_idx]->SetBlink();
}

void ASelectCharacter::MoveSelectUI(int _idx)
{
	for (int j = 0, size = ARRAYSIZE(DISABLE_IDXS); j < size; ++j)
	{
		if (_idx == DISABLE_IDXS[j])
		{
			_idx = ++SelectedIdx;
			break;
		}
	}

	RSelect->SetRelativeLocation(ImageList[_idx]->GetActorLocation() + SelectMargin);
	RPlayer1->SetRelativeLocation(RSelect->GetRelativeLocation() + SelectMargin);
}

void ASelectCharacter::OnOffObjs(bool _isActive)
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (i != SelectedIdx)
		{
			ImageList[i]->SetActive(_isActive);
		}
	}
}

/* Fsm start functions */
void ASelectCharacter::OnFinish()
{
	BtnOk->SetBlinkState(EBlinkState::SELECTED);
	Fsm.ChangeState(ESceneState::END);

	if (EndFuntion != nullptr)
	{
		EndFuntion();
	}
}

void ASelectCharacter::OnSelect()
{
	OnOffObjs(true);
}

void ASelectCharacter::OnSelectMoveForward()
{
	OnOffObjs(false);

	LocOrg = ImageList[SelectedIdx]->GetActorLocation();

	ImageList[SelectedIdx]->SetSelect();
	LocDst = FVector{ 0.f, 0.f };
}

void ASelectCharacter::OnSelectMoveBackward()
{
	BtnOk->SetActive(false);

	ImageList[SelectedIdx]->SetIdle();
	BtnOk->SetBlinkState(EBlinkState::OFF);
	LocDst = LocOrg;
}

void ASelectCharacter::OnWaitOk()
{
	BtnOk->SetActive(true);
	BtnOk->SetActorRotation({ 0.f, 90.f, 0.f });
	BtnOk->SetBlinkState(EBlinkState::BLINK);
}

/* Fsm update functions */
void ASelectCharacter::Selecting(float _deltaTime)
{
	static float elapsedSecs = 1.f;
	elapsedSecs += _deltaTime;
	if (elapsedSecs > .5f)
	{
		elapsedSecs = 0.f;

		int idx = rand() % SIZE;
		RunBlink(idx);
	}

	if (UEngineInput::IsDown(VK_LEFT))
	{
		if (SelectedIdx > 0)
		{
			MoveSelectUI(--SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		if (SelectedIdx < SIZE - 1)
		{
			MoveSelectUI(++SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_UP))
	{
		if (SelectedIdx >= HALF_SIZE)
		{
			SelectedIdx -= 4;
			MoveSelectUI(SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		if (SelectedIdx < HALF_SIZE)
		{
			SelectedIdx += 4;
			MoveSelectUI(SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_SPACE))
	{
		Fsm.ChangeState(ESceneState::SELECT_MOVING);
	}
}

void ASelectCharacter::Moving(float _deltaTime)
{
	FVector loc = ImageList[SelectedIdx]->GetActorLocation();
	FVector dir = LocDst - loc;
	FVector dirDst = dir;
	dirDst.Normalize();
	dirDst *= (1000.f * _deltaTime);
	dirDst.Z = 0.f;

	if (abs(dir.X) < 1 && abs(dir.Y) < 1 || abs(dirDst.X) > abs(dir.X) && abs(dirDst.Y) > abs(dir.Y))
	{
		ImageList[SelectedIdx]->SetActorLocation(LocDst);
		MoveSelectUI(SelectedIdx);

		if (static_cast<ESceneState>(Fsm.GetCurIState()) == ESceneState::SELECT_MOVING)
		{
			Fsm.ChangeState(ESceneState::WAIT_OK);
		}
		else
		{
			Fsm.ChangeState(ESceneState::SELECT);
		}
		return;
	}

	ImageList[SelectedIdx]->AddActorLocation(dirDst);
	MoveSelectUI(SelectedIdx);
}

void ASelectCharacter::Waiting(float _deltaTime)
{
	float rotY = BtnOk->GetTransform().Rotation.Y;
	if (rotY > 0)
	{
		BtnOk->AddActorRotation({ 0.f, -180.f * _deltaTime, 0.f });
	}

	if (UEngineInput::IsDown(VK_ESCAPE))
	{
		Fsm.ChangeState(ESceneState::SELECT_MOVING_REVERSE);
	}
	else if (UEngineInput::IsDown(VK_SPACE))
	{
		Fsm.ChangeState(ESceneState::FINISH);
	}
}

