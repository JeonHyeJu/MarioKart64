#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/CameraActor.h>
#include <EnginePlatform/EngineInput.h>

ATestMap::ATestMap()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	Renderer->SetupAttachment(RootComponent);

	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 100.0f, -500.0f, 1.0f });
}

ATestMap::~ATestMap()
{

}

void ATestMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ATestMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	if (UEngineInput::IsPress('T'))
	{
		Camera->AddRelativeLocation({ 0.f, 100.f * _deltaTime, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('G'))
	{
		Camera->AddRelativeLocation({ 0.f, -100.f * _deltaTime, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, 100.f * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, -100.f * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('A'))
	{
		Camera->AddRelativeLocation({ -100.f * _deltaTime, 0.0f, 0.f, 1.0f });
		//AddRelativeLocation(FVector{ 100.0f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddRelativeLocation({ 100.f * _deltaTime, 0.0f, 0.f, 1.0f });
		//AddRelativeLocation(FVector{ 100.0f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		Camera->AddActorRotation(FVector{ -180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('E'))
	{
		Camera->AddActorRotation(FVector{ 180.f * _deltaTime, 0.f, 0.f });
	}
}
