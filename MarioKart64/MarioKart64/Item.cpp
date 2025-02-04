#include "PreCompile.h"
#include "ItemBox.h"
#include "Item.h"
#include "CData.h"
#include "CubeRenderer.h"
#include "BaseMap.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/Collision.h>

AItem::AItem()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
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
	else if (ItemType == EItemType::BOWSER_SHELL)
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

void AItem::Init(const EItemType& _itemType, ABaseMap* _mapPtr, int _navIdx)
{
	ItemType = _itemType;
	MapPtr = _mapPtr;
	NavIdx = _navIdx;

	switch (ItemType)
	{
		case EItemType::GREEN_SHELL:
		case EItemType::GREEN_SHELLS:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 16, 23, .1f);
			renderer->ChangeAnimation("Run");
			
			_CreateCollision();
			Collision->SetCollisionProfileName("SHELL");
			break;
		}
		case EItemType::RED_SHELL:
		case EItemType::RED_SHELLS:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 25, 32, .1f);
			renderer->ChangeAnimation("Run");
			//renderer->SetRelativeLocation({ -Size * .5f, Size * .5f, 0.f });

			_CreateCollision();
			Collision->SetCollisionProfileName("SHELL");
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
			//renderer->SetRelativeLocation({ 0.f, -Size * .5f, 0.f });

			_CreateCollision();
			Collision->SetCollisionProfileName("BANANA");
			break;
		}
		case EItemType::STAR:
			break;
		case EItemType::THUNDER:
			break;
		case EItemType::GHOST:
			break;
		case EItemType::FAKE_ITEMBOX:
		{
			_CreateCubeRenderer();

			_CreateCollision();
			Collision->SetCollisionProfileName("FAKE_ITEMBOX");

			const float SIZE = AItemBox::SIZE;
			Collision->SetScale3D({ SIZE, SIZE, SIZE });
			Collision->SetRelativeLocation({ 0.f, 0.f, 0.f });
			break;
		}
		case EItemType::BOWSER_SHELL:
		{
			std::shared_ptr<USpriteRenderer> renderer = _CreateSpriteRenderer();
			renderer->CreateAnimation("Run", "Items.png", 33, 40, .1f);
			renderer->ChangeAnimation("Run");

			_CreateCollision();
			Collision->SetCollisionProfileName("SHELL");
			break;
		}
		default:
			break;
	}
}

void AItem::_CreateCollision()
{
	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetCollisionType(ECollisionType::Sphere);
	Collision->SetupAttachment(RootComponent);
	Collision->SetScale3D({ Size, Size, Size });
	Collision->SetRelativeLocation({ 0.f, Size * .5f, 0.f });
}

std::shared_ptr<USpriteRenderer> AItem::_CreateSpriteRenderer()
{
	std::shared_ptr<USpriteRenderer> renderer = CreateDefaultSubObject<USpriteRenderer>();
	renderer->SetOrder(0);
	renderer->SetRelativeLocation({ 0.f, Size * .5f, 0.f });
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
	if (elapsedSecs > 3.f)
	{
		elapsedSecs = 0.f;
		Destroy();
	}
	
	float fDist = 0.f;
	FVector lastVec = FVector::ZERO;
	const FTransform& trfm = GetTransform();
	float dx = FPhysics::GetDeltaX(Velocity, Acceleration, _deltaTime);
	Velocity = FPhysics::GetVf(Velocity, Acceleration, _deltaTime);

	bool hasFloor = CheckCollision(trfm.Location + FVector{ 0.f, GRAVITY_FORCE, 0.f }, NavIdx, fDist);
	if (hasFloor)
	{
		// for fDist. Do not remove
		float gravityY = GRAVITY_FORCE * _deltaTime;
		CheckCollision(trfm.Location + FVector{ 0.f, gravityY, 0.f }, NavIdx, fDist);

		lastVec = Direction * dx;
		lastVec.Y += gravityY + fDist;
	}
	else
	{
		Destroy();
	}

	AddActorLocation(lastVec);
}

void AItem::RunningFakeItem(float _deltaTime)
{
	float rot = AItemBox::ROTATION_DEG * _deltaTime;
	AddActorRotation({ 0.f, rot, rot });
}

bool AItem::CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist)
{
	bool isCollided = false;
	const FTransform& trfmObj = MapPtr->GetTransform();

	const std::vector<SNavData>& navDatas = MapPtr->GetNavData();
	SNavData nd = navDatas[_refIdx];
	isCollided = nd.Intersects(_loc, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, _refDist);

	if (!isCollided)
	{
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(_loc, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, _refDist);
			if (isCollided)
			{
				_refIdx = linkedIdx;
				break;
			}
		}
	}

	return isCollided;
}