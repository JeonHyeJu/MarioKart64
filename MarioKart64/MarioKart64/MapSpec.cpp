#include "PreCompile.h"
#include "MapSpec.h"
#include "SelectButton.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>

AMapSpec::AMapSpec()
{
	RootComponent = CreateDefaultSubObject<UDefaultSceneComponent>();

	const float SPEED = 2.f;
	RMapImg = CreateDefaultSubObject<USpriteRenderer>();
	RMapImg->CreateAnimation("Slide_0", SPRITE_NAME, { 5, 6, 7, 8 }, SPEED);
	RMapImg->CreateAnimation("Slide_1", SPRITE_NAME, { 23, 24, 25, 26 }, SPEED);
	RMapImg->CreateAnimation("Slide_2", SPRITE_NAME, { 41, 42, 43, 44 }, SPEED);
	RMapImg->CreateAnimation("Slide_3", SPRITE_NAME, { 59, 60, 61, 62 }, SPEED);
	RMapImg->SetAutoScaleRatio(5.f);
	RMapImg->SetupAttachment(RootComponent);
	RMapImg->SetRelativeLocation({ -500.f, -300.f, -10.f });

	float width = -1.f;
	float height = -1.f;
	const float INIT_X = 200.f;
	const float INIT_Y = -150.f;
	for (int i = 0; i < SIZE; ++i)
	{
		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		ptr->Init(SPRITE_NAME, 9+i, 5.f);	// Temp

		if (width < 0)
		{
			FVector scale = ptr->GetRealScaleOfSprite();
			width = scale.X;
			height = scale.Y;
		}

		ptr->SetActorLocation({ INIT_X, INIT_Y - i * (height + 20.f), -5.f });
		ptr->AttachToActor(this);
		TrackList[i] = ptr.get();
	}

	Init(3);
}

AMapSpec::~AMapSpec()
{

}

void AMapSpec::BeginPlay()
{
	AActor::BeginPlay();
}

void AMapSpec::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}

void AMapSpec::Init(int _idx)
{
	RMapImg->ChangeAnimation("Slide_" + std::to_string(_idx));

	for (int i = 0; i < SIZE; ++i)
	{
		TrackList[i]->SetImage(SPRITE_NAME, 9 + (_idx * 18) + i);
	}
}