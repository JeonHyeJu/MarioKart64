#include "PreCompile.h"
#include "Item.h"
#include "CData.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/Collision.h>

AItem::AItem()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(0);

	Renderer->CreateAnimation("Run", "Items.png", 16, 23, .5f);
	Renderer->ChangeAnimation("Run");
	Renderer->SetupAttachment(RootComponent);

	Size = Renderer->GetWorldScale3D().X;

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetCollisionType(ECollisionType::Sphere);
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("Item");
	Collision->SetScale3D({ Size, Size, Size });
	Collision->AddRelativeLocation({ Size * .5f, -Size * .5f, 0.f });

	// TODO: nav mesh
}

AItem::~AItem()
{
}

void AItem::BeginPlay()
{
	AActor::BeginPlay();
}

void AItem::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	if (Velocity < 0)
	{
		return;
	}

	static float elapsedSecs = 0.f;
	elapsedSecs += _deltaTime;
	if (elapsedSecs > 5.f)
	{
		elapsedSecs = 0.f;
		Destroy();
	}

	float dx = FPhysics::GetDeltaX(Velocity, Acceleration, _deltaTime);
	Velocity = FPhysics::GetVf(Velocity, Acceleration, _deltaTime);
	AddActorLocation(Direction * dx);
}

void AItem::SetDirection(const FVector& _dir)
{
	Direction = _dir;
	Direction.Normalize();
}