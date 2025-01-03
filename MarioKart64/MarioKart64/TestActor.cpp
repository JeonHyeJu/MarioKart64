#include "PreCompile.h"
#include "TestActor.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include "MyCustomRenderer.h"

TestActor::TestActor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<MyCustomRenderer>();
	Renderer->SetupAttachment(RootComponent);

	Renderer->SetRelativeScale3D({100.0f, 100.0f, 100.0f});

	LogoRenderer = CreateDefaultSubObject<USpriteRenderer>();
	LogoRenderer->SetupAttachment(RootComponent);
	LogoRenderer->SetAutoScaleRatio(5.0f);

	LogoRenderer->CreateAnimation("Idle", "Mario.png", 0, 3, 0.5f);
	LogoRenderer->CreateAnimation("Move", "Mario.png", 4, 16, 0.3f);
	LogoRenderer->ChangeAnimation("Move");
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

	std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetCamera(0);

	UEngineCore::GetMainWindow().GetMousePos();

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

	if (UEngineInput::IsPress('F'))
	{
		LogoRenderer->ColorData.MulColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (UEngineInput::IsPress('E'))
	{
		LogoRenderer->ColorData.PlusColor += float4(1.0f, 1.0f, 1.0f, 1.0f) * _DeltaTime;
		LogoRenderer->ColorData.PlusColor.W += _DeltaTime;
	}

	if (UEngineInput::IsPress('R'))
	{
		LogoRenderer->ColorData.PlusColor -= float4(1.0f, 1.0f, 1.0f, 1.0f) * _DeltaTime;
		LogoRenderer->ColorData.PlusColor.W -= _DeltaTime;

		// Child->SetWorldLocation(FVector{ 100.0f, 0.0f , 0.0f });
	}
}