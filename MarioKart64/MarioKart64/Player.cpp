#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

APlayer::APlayer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();

	// Temp
	Renderer->CreateAnimation("Idle", "Mario.png", 0, 0, 0.1f);
	{
		USpriteRenderer::FrameAnimation* anim = Renderer->FindAnimation("Idle");
		anim->IsAutoScale = true;
		anim->AutoScaleRatio = 4.0f;
	}

	Renderer->ChangeAnimation("Idle");

	Renderer->SetupAttachment(RootComponent);
}

APlayer::~APlayer()
{
}

void APlayer::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
	APawn::Tick(_deltaTime);

	if (UEngineInput::IsPress(VK_UP))
	{
		AddRelativeLocation({ 0.f, 0.f, 100.f * _deltaTime, 1.0f });
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
		AddRelativeLocation({ 0.f, 0.f, -100.f * _deltaTime, 1.0f });
	}

	if (UEngineInput::IsPress(VK_LEFT))
	{
		AddRelativeLocation({ -100.f * _deltaTime, 0.f, 0.f, 1.0f });
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddRelativeLocation({ 100.f * _deltaTime, 0.f, 0.f, 1.0f });
	}
}