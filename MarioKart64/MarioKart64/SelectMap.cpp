#include "PreCompile.h"
#include "SelectMap.h"
#include "SelectButton.h"
#include "MapSpec.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ASelectMap::ASelectMap()
{
	const char* SPRITE_NAME = "TrackIcons.png";

	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBg = CreateDefaultSubObject<USpriteRenderer>();
	RBg->SetSprite("Background", 2);
	RBg->SetAutoScaleRatio(.75f);
	RBg->SetupAttachment(RootComponent);
	
	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RTitle->SetSprite(SPRITE_NAME, 0);
	RTitle->SetAutoScaleRatio(.75f);
	RTitle->SetAutoScaleRatio(5.f);
	RTitle->SetRelativeLocation({ 0.f, 380.f, -5.f });
	RTitle->SetupAttachment(RootComponent);
	
	{
		const float SCALE_RATIO = 5.f;
		int idxs[4] = { 2, 21, 39, 57 };
		float width = -1.f;
		float height = 140.f;
		const float INIT_X = -480.f;
		for (int i = 0; i < SIZE; ++i)
		{
			std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
			ptr->Init(SPRITE_NAME, idxs[i], SCALE_RATIO);

			if (width < 0)
			{
				FVector scale = ptr->GetRealScaleOfSprite();
				width = scale.X;
			}

			ptr->SetActorLocation({ INIT_X + i * width, height, -5.f });
			ptr->AttachToActor(this);
			MainRects[i] = ptr.get();
		}
	}

	MapSpec = GetWorld()->SpawnActor<AMapSpec>();
	MapSpec->SetActorLocation({ 100.f, 100.f, -5.f });
	MapSpec->AttachToActor(this);

	Hover(CurSelectedIdx);
}

ASelectMap::~ASelectMap()
{

}

void ASelectMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ASelectMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	Selecting(_deltaTime);
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
		if (CurSelectedIdx < SIZE - 1)
		{
			Hover(CurSelectedIdx + 1);
		}
	}
}