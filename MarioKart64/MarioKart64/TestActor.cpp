#include "PreCompile.h"
#include "TestActor.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/DefaultSceneComponent.h>

TestActor::TestActor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	LogoRenderer = CreateDefaultSubObject<USpriteRenderer>();

	LogoRenderer->CreateAnimation("Idle", "Mario.png", 0, 0, 0.1f);
	{
		USpriteRenderer::FrameAnimation* Animation = LogoRenderer->FindAnimation("Idle");
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 4.0f;
	}

	LogoRenderer->CreateAnimation("Move", "Mario.png", 1, 4, 0.3f);
	{
		USpriteRenderer::FrameAnimation* Animation = LogoRenderer->FindAnimation("Move");
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 4.0f;
	}

	LogoRenderer->ChangeAnimation("Idle");

	LogoRenderer->SetRelativeScale3D({50, 50, 1.0f});
	LogoRenderer->SetupAttachment(RootComponent);

	//Child = CreateDefaultSubObject<USpriteRenderer>();
	//Child->SetSprite("Mario.png", 2);
	//// 부모의 스케일이 나에게 영향을 주면서 나는 100이 아닐수가 있다
	//Child->SetRelativeLocation({100.0f, 0.0f, 0.0f});
	//Child->SetScale3D({ 50.0f, 50.0f, 1.0f });
	//// Child->SetScale3D({ 50.0f, 50.0f, 1.0f });
	//Child->SetupAttachment(RootComponent);
}

TestActor::~TestActor()
{
}

void TestActor::BeginPlay()
{
	AActor::BeginPlay();
}

void TestActor::Tick(float _DeltaTime)
{

	AActor::Tick(_DeltaTime);

	if (UEngineInput::IsPress('A'))
	{
		AddRelativeLocation(FVector{ -100.0f * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		AddRelativeLocation(FVector{ 100.0f * _DeltaTime, 0.0f, 0.0f });
	}

	if (UEngineInput::IsPress('W'))
	{
		AddRelativeLocation(FVector{ 0.0f, 100.0f * _DeltaTime, 0.0f });
	}

	if (UEngineInput::IsPress('S'))
	{
		AddRelativeLocation(FVector{ 0.0f, -100.0f * _DeltaTime, 0.0f });
	}

	if (UEngineInput::IsPress('Q'))
	{
		AddActorRotation(FVector{ 0.0f, 0.0f , 360.0f * _DeltaTime });
	}

	if (UEngineInput::IsPress('E'))
	{
		LogoRenderer->ChangeAnimation("Move");
	}

	if (UEngineInput::IsPress('R'))
	{
		// Child->SetWorldLocation(FVector{ 100.0f, 0.0f , 0.0f });
	}
}