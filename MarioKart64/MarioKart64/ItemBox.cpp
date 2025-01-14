#include "PreCompile.h"
#include "ItemBox.h"
#include "CubeRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/Collision.h>

const float AItemBox::SIZE = 50.f;
const float AItemBox::ROTATION_DEG = 30.f;

AItemBox::AItemBox()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BoxRenderer = CreateDefaultSubObject<CubeRenderer>();
	BoxRenderer->SetScale3D({ SIZE, SIZE, SIZE });
	BoxRenderer->SetOrder(0);
	BoxRenderer->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetCollisionType(ECollisionType::Sphere);
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("ItemBox");
	Collision->SetScale3D({ SIZE, SIZE, SIZE });
}

AItemBox::~AItemBox()
{
}

void AItemBox::BeginPlay()
{
	AActor::BeginPlay();
}

void AItemBox::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	float rot = ROTATION_DEG* _deltaTime;
	BoxRenderer->AddLocalRotation({ 0.f, rot, rot });
}
