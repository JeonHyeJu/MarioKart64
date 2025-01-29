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

	// Temp
	/*const int TURN_SIZE = 31;
	std::vector<int> idxs(TURN_SIZE, 0);
	std::vector<float> times(TURN_SIZE, 0.2f);
	for (size_t i = 0; i < TURN_SIZE; ++i)
	{
		idxs[i] = static_cast<int>(i * 36);
	}*/

	Renderer->CreateAnimation("Idle", "Lakitu.png", 0, 0, 1.f);
	/*Renderer->CreateAnimation("TurnL", "Mario.png", idxs, times, true);
	Renderer->CreateAnimation("TurnR", "Mario.png", idxs, times, true);*/

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