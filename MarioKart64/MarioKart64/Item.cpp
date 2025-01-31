#include "PreCompile.h"
#include "ItemBox.h"
#include "Item.h"
#include "CData.h"
#include "CubeRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/Collision.h>

AItem::AItem()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetCollisionType(ECollisionType::Sphere);
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("Item");
	Collision->SetScale3D({ Size, Size, Size });

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

	// TODO: FSM
	if (ItemType <= EItemType::GREEN_SHELLS)
	{
		RunningShell(_deltaTime);
	}
	else if (ItemType <= EItemType::RED_SHELLS)
	{
		RunningShell(_deltaTime);
	}
	else if (ItemType == EItemType::RED_SHELLS)
	{
		RunningShell(_deltaTime);
	}
	else if (ItemType <= EItemType::GOLD_MUSHROOM)
	{
		// Nothing to do in Item..
	}
	else if (ItemType <= EItemType::BANANAS)
	{
		// Nothing to do in Item..
	}
	else if (ItemType == EItemType::FAKE_ITEMBOX)
	{
		RunningFakeItem(_deltaTime);
	}
}

void AItem::Init(const EItemType& _itemType)
{
	ItemType = _itemType;

	switch (ItemType)
	{
		case EItemType::GREEN_SHELL:
		case EItemType::GREEN_SHELLS:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 16, 23, .1f);
			renderer->ChangeAnimation("Run");
			break;
		}
		case EItemType::RED_SHELL:
		case EItemType::RED_SHELLS:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 25, 32, .1f);
			renderer->ChangeAnimation("Run");
			renderer->SetRelativeLocation({ -Size * .5f, Size * .5f, 0.f });
			break;
		}
		case EItemType::MUSHROOM:
		case EItemType::MUSHROOM2:
		case EItemType::MUSHROOM3:
			break;
		case EItemType::GOLD_MUSHROOM:
			break;
		case EItemType::BANANA:
		case EItemType::BANANAS:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->SetSprite("Items.png", 42);
			renderer->SetAutoScale(false);
			renderer->SetScale3D({ Size, Size * .5f, Size });
			renderer->SetRelativeLocation({ 0.f, -Size * .5f, 0.f });
			break;
		}
		case EItemType::STAR:
			break;
		case EItemType::THUNDER:
			break;
		case EItemType::GHOST:
			break;
		case EItemType::FAKE_ITEMBOX:
			_CreateCubeRenderer();
			break;
		case EItemType::BOWSER_SHELL:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 33, 40, .1f);
			renderer->ChangeAnimation("Run");
			break;
		}
		default:
			break;
	}
}

std::shared_ptr<USpriteRenderer> AItem::_CreateSpriteRenderer()
{
	std::shared_ptr<USpriteRenderer> renderer = CreateDefaultSubObject<USpriteRenderer>();
	renderer->SetOrder(0);
	renderer->SetupAttachment(RootComponent);
	return renderer;
}

std::shared_ptr<CubeRenderer> AItem::_CreateCubeRenderer()
{
	const float SIZE = AItemBox::SIZE;
	std::shared_ptr<CubeRenderer> renderer = CreateDefaultSubObject<CubeRenderer>();
	renderer->SetScale3D({ SIZE, SIZE, SIZE });
	renderer->SetOrder(0);
	renderer->SetupAttachment(RootComponent);

	Collision->SetScale3D({ SIZE, SIZE, SIZE });
	Collision->SetRelativeLocation({ 0.f, 0.f, 0.f });

	return renderer;
}

void AItem::SetDirection(const FVector& _dir)
{
	Direction = _dir;
	Direction.Normalize();
}

void AItem::SetInitVelocity(float _velocity)
{
	Velocity = _velocity;
}

void AItem::RunningShell(float _deltaTime)
{
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

void AItem::RunningFakeItem(float _deltaTime)
{
	float rot = AItemBox::ROTATION_DEG * _deltaTime;
	AddActorRotation({ 0.f, rot, rot });
}