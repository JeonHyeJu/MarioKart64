#include "PreCompile.h"
#include "SelectMap.h"
#include "SelectButton.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ASelectMap::ASelectMap()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBg = CreateDefaultSubObject<USpriteRenderer>();
	RBg->SetSprite("Background", 2);
	RBg->SetAutoScaleRatio(.75f);
	RBg->SetupAttachment(RootComponent);
	
	RTitle = CreateDefaultSubObject<USpriteRenderer>();
	RTitle->SetSprite("TrackIcons.png", 0);
	RTitle->SetAutoScaleRatio(.75f);
	RTitle->SetAutoScaleRatio(5.f);
	RTitle->SetRelativeLocation({ -440.f, 300.f, -5.f });
	RTitle->SetupAttachment(RootComponent);

	const char* SPRITE_NAME = "TrackIcons.png";
	const float SCALE_RATIO = 5.f;
	int idxs[4] = {2, 21, 39, 57 };
	float width = -1.f;
	float height = 200.f;
	const float INIT_X = -640.f;
	for (int i = 0; i < SIZE; ++i)
	{
		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		ptr->Init(SPRITE_NAME, idxs[i], SCALE_RATIO);

		if (width < 0)
		{
			FVector scale = ptr->GetRealScaleOfSprite();
			width = scale.X;
		}
		
		ptr->SetActorLocation({ INIT_X + i * width, height, -105.f });
		ptr->AttachToActor(this);
		MainRects[i] = ptr.get();
	}
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
}
