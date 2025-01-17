#include "PreCompile.h"
#include "SelectCharacter.h"
#include "SelectButton.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ASelectCharacter::ASelectCharacter()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBg = CreateDefaultSubObject<USpriteRenderer>();
	RBg->SetSprite("Background", 1);
	RBg->SetAutoScaleRatio(.75f);
	RBg->SetupAttachment(RootComponent);

	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RTitle->SetSprite("SelectCharacter", 0);
	RTitle->SetAutoScaleRatio(.6f);
	RTitle->SetupAttachment(RootComponent);
	RTitle->SetRelativeLocation({ 0.f, 450.f, - 5.f });

	BtnOk = GetWorld()->SpawnActor<ASelectButton>();
	BtnOk->Init("SelectGame", 15, .6f);
	BtnOk->AttachToActor(this);
	BtnOk->SetActorLocation({ 840.f, -460.f, -25.f });

	RSelect = CreateDefaultSubObject<USpriteRenderer>();
	RSelect->SetSprite("SelectCharacter.png", 164);
	RSelect->SetAutoScaleRatio(5.f);
	RSelect->SetRelativeLocation({ 0.f, 0.f, -15.f });

	RPlayer1 = CreateDefaultSubObject<USpriteRenderer>();
	RPlayer1->SetSprite("SelectCharacter.png", 160);
	RPlayer1->SetAutoScaleRatio(5.f);
	RPlayer1->SetRelativeLocation({ 0.f, 0.f, -15.f });

	const int STRIDE = 20;
	float width = -1.f;
	float height = -1.f;
	const float MARGIN_X = 30.f;
	const float INITAL_X = -640.f;
	const float ANIM_SEC = .1f;

	const char* SPRITE_NAME = "SelectCharacter.png";
	std::vector<int> _idlIdxs = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 };
	std::vector<float> _idlSecs(_idlIdxs.size(), ANIM_SEC);
	_idlSecs[_idlSecs.size() - 1] = 1.f;

	std::vector<int> _selIdxs = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	for (int i = 0; i < SIZE; ++i)
	{
		// Picture
		std::shared_ptr<USpriteRenderer> ptr = CreateDefaultSubObject<USpriteRenderer>();

		std::vector<int> idleIdxs = _idlIdxs;
		std::vector<int> selectIdxs = _selIdxs;
		int sadIdx = 16 + (STRIDE * i);
		
		for (int& val : idleIdxs)
		{
			val += STRIDE * i;
		}
		for (int& val : selectIdxs)
		{
			val += STRIDE * i;
		}

		ptr->CreateAnimation("Idle", SPRITE_NAME, idleIdxs[0], idleIdxs[0], 1.f);
		ptr->CreateAnimation("Blink", SPRITE_NAME, idleIdxs, _idlSecs, false);
		ptr->CreateAnimation("Select", SPRITE_NAME, selectIdxs, ANIM_SEC, false);
		ptr->CreateAnimation("Sad", SPRITE_NAME, sadIdx, sadIdx, ANIM_SEC);
		ptr->ChangeAnimation("Idle");
		ptr->SetAutoScaleRatio(5.f);
		ptr->SetupAttachment(RootComponent);

		if (width < 0)
		{
			FVector scale = ptr->GetRealScaleOfSprite();
			width = scale.X;
			height = scale.Y;
		}

		float x = INITAL_X + (width + MARGIN_X) * (i % 4);
		float y = (i < 4) ? 0.f : -height - MARGIN_Y;
		FVector loc{ x, y, -10.f };

		ptr->SetRelativeLocation(loc);
		ImageList[i] = ptr;

		// Name
		const int INIT_NAME_IDX = 17;
		std::shared_ptr<USpriteRenderer> ptrName = CreateDefaultSubObject<USpriteRenderer>();
		ptrName->SetSprite(SPRITE_NAME, INIT_NAME_IDX + (i* STRIDE));
		ptrName->SetAutoScaleRatio(5.f);
		ptrName->SetupAttachment(RootComponent);
		ptrName->SetRelativeLocation(loc + FVector{0.f, -MARGIN_Y * .75f, -1.f });
		NameList[i] = ptrName;
	}

	RSelect->SetRelativeLocation(ImageList[0]->GetRelativeLocation() + FVector{0.f, 0.f, -1.f});
	RPlayer1->SetRelativeLocation(RSelect->GetRelativeLocation() + FVector{0.f, 0.f, -1.f});

	RSelect->SetupAttachment(RootComponent);
	RPlayer1->SetupAttachment(RootComponent);
}

ASelectCharacter::~ASelectCharacter()
{

}

void ASelectCharacter::BeginPlay()
{
	AActor::BeginPlay();
	srand(static_cast<unsigned int>(time(nullptr)));
}

void ASelectCharacter::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	static float elapsedSecs = 1.f;

	if (SceneState == ESceneState::SELECT)
	{
		elapsedSecs += _deltaTime;
		if (elapsedSecs > .5f)
		{
			elapsedSecs = 0.f;

			int idx = rand() % SIZE;
			RunBlink(idx);
		}

		Selecting(_deltaTime);
	}
	else if (SceneState == ESceneState::SELECT_MOVING || SceneState == ESceneState::SELECT_MOVING_REVERSE)
	{
		Moving(_deltaTime);
	}
	else if (SceneState == ESceneState::WAIT_OK)
	{
		Waiting(_deltaTime);
	}
	else if (SceneState == ESceneState::FINISH)
	{
		OnFinish();
	}
}

void ASelectCharacter::RunBlink(int _idx)
{
	ImageList[_idx]->ChangeAnimation("Blink", true);
}

void ASelectCharacter::Move(int _idx)
{
	RSelect->SetRelativeLocation(ImageList[_idx]->GetRelativeLocation() + FVector{ 0.f, 0.f, -1.f });
	RPlayer1->SetRelativeLocation(RSelect->GetRelativeLocation() + FVector{ 0.f, 0.f, -1.f });
}

void ASelectCharacter::OffObjs()
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (i != SelectedIdx)
		{
			ImageList[i]->SetActive(false);
			NameList[i]->SetActive(false);
		}
	}

	LocOrg = ImageList[SelectedIdx]->GetRelativeLocation();
	LocOrgName = NameList[SelectedIdx]->GetRelativeLocation();
}

void ASelectCharacter::Moving(float _deltaTime)
{
	FVector loc = ImageList[SelectedIdx]->GetRelativeLocation();
	FVector dir = LocDst - loc;	// TODO
	FVector dirDst = dir;
	dirDst.Normalize();
	dirDst *= (1000.f * _deltaTime);
	dirDst.Z = 0.f;
	float len2d = sqrtf(dir.X * dir.X + dir.Y * dir.Y);

	/*OutputDebugStringA(("1. dir: " + std::to_string(dir.X) + ", " + std::to_string(dir.Y) + "\n").c_str());
	OutputDebugStringA(("2. dirDst: " + std::to_string(dirDst.X) + ", " + std::to_string(dirDst.Y) + "\n").c_str());*/

	if (SceneState == ESceneState::SELECT_MOVING)
	{
		if (dir.X < 1 && dir.Y < 1)
		{
			BtnOk->SetBlinkState(EBlinkState::BLINK);
			SceneState = ESceneState::WAIT_OK;
		}
	}

	if (SceneState == ESceneState::SELECT_MOVING_REVERSE)
	{
		// Temp
		if (abs(dir.X) < 1 && abs(dir.Y) < 1 || abs(dirDst.X) > abs(dir.X) && abs(dirDst.Y) > abs(dir.Y))
		{
			SceneState = ESceneState::SELECT;
		}
	}

	ImageList[SelectedIdx]->AddRelativeLocation(dirDst);
	NameList[SelectedIdx]->AddRelativeLocation(dirDst);
	Move(SelectedIdx);
}

void ASelectCharacter::Selecting(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_LEFT))
	{
		if (SelectedIdx > 0)
		{
			Move(--SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		if (SelectedIdx < SIZE - 1)
		{
			Move(++SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_UP))
	{
		if (SelectedIdx >= HALF_SIZE)
		{
			SelectedIdx -= 4;
			Move(SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		if (SelectedIdx < HALF_SIZE)
		{
			SelectedIdx += 4;
			Move(SelectedIdx);
		}
	}
	else if (UEngineInput::IsDown(VK_SPACE))
	{
		OffObjs();
		ImageList[SelectedIdx]->ChangeAnimation("Select");
		LocDst = FVector{ -125.f, -125.f };
		SceneState = ESceneState::SELECT_MOVING;
	}
}

void ASelectCharacter::Waiting(float _deltaTime)
{
	if (UEngineInput::IsDown(VK_ESCAPE))
	{
		ImageList[SelectedIdx]->ChangeAnimation("Idle");
		BtnOk->SetBlinkState(EBlinkState::OFF);
		LocDst = LocOrg;
		SceneState = ESceneState::SELECT_MOVING_REVERSE;
	}
	else if (UEngineInput::IsDown(VK_SPACE))
	{
		BtnOk->SetBlinkState(EBlinkState::SELECTED);
		SceneState = ESceneState::FINISH;
	}
}

void ASelectCharacter::OnFinish()
{
	SceneState = ESceneState::END;

	if (EndFuntion != nullptr)
	{
		EndFuntion();
	}
}