#include "PreCompile.h"
#include "Skybox.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include "ColorRenderer.h"

ASkybox::ASkybox()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// TODO: check camera's value

	Front = CreateDefaultSubObject<USpriteRenderer>();
	//Front->SetTexture("PinkSky.png");
	Front->SetSprite("Background", 0);
	Front->SetAutoScaleRatio(10.f);
	//Front->BillboardOn();	// TODO: ..?
	Front->SetupAttachment(RootComponent);
	Front->SetRelativeLocation({0.f, 0.f, 10000.f});
	Front->SetOrder(1);

	/*Left = CreateDefaultSubObject<ColorRenderer>();
	Left->SetupAttachment(RootComponent);
	Left->SetRelativeScale3D(size);
	Left->SetRotation({0.f, -90.f, 0.f});
	Left->SetRelativeLocation({ -10000.f, 0.f, 0.f });
	Left->SetColor({ 1.f, 0.f, 1.f, 1.f });
	Left->SetOrder(1);

	Right = CreateDefaultSubObject<ColorRenderer>();
	Right->SetupAttachment(RootComponent);
	Right->SetRelativeScale3D(size);
	Right->SetRotation({ 0.f, 90.f, 0.f });
	Right->SetRelativeLocation({ 10000.f, 0.f, 0.f });
	Right->SetColor({ 1.f, 0.f, 1.f, 1.f });
	Right->SetOrder(1);

	Back = CreateDefaultSubObject<ColorRenderer>();
	Back->SetupAttachment(RootComponent);
	Back->SetRelativeScale3D(size);
	Back->SetRelativeLocation({ 0.f, 0.f, -9000.f });
	Back->SetColor({ 1.f, 0.f, 1.f, 1.f });
	Back->SetOrder(1);*/
}

ASkybox::~ASkybox()
{
}

void ASkybox::BeginPlay()
{
	AActor::BeginPlay();
}

void ASkybox::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}
