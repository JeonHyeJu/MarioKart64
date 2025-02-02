#include "PreCompile.h"
#include "Lakitu.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>

ALakitu::ALakitu()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(0);

	const char* SPRITE_NAME = "Lakitu.png";
	Renderer->CreateAnimation("Idle", SPRITE_NAME, 0, 0, 1.f);
	Renderer->CreateAnimation("Start", SPRITE_NAME, 0, 27, .1f, false);

	Renderer->ChangeAnimation("Idle");
	Renderer->SetupAttachment(RootComponent);
}

ALakitu::~ALakitu()
{
}

void ALakitu::BeginPlay()
{
	AActor::BeginPlay();
}

void ALakitu::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}

void ALakitu::Launch()
{
	Renderer->ChangeAnimation("Start");
}

bool ALakitu::IsAnimEnd()
{
	return Renderer->IsCurAnimationEnd();
}

int ALakitu::GetCurAnimIdx() const
{
	return Renderer->GetCurAnimIdx();
}