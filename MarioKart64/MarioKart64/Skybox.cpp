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
	FVector size{ 50000.f, 20000.f, 1.f };

	Front = CreateDefaultSubObject<ColorRenderer>();
	Front->SetupAttachment(RootComponent);
	Front->SetRelativeScale3D(size);
	Front->SetRelativeLocation({0.f, 1000.f, 9000.f});
	Front->SetColor({1.f, 0.f, 1.f, 1.f});
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

	MakeClouds();
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

// Temp
void ASkybox::MakeClouds()
{
	srand(static_cast<int>(time(NULL)));
	int cnt = rand() % 10 + 3;

	for (int i = 0; i < cnt; ++i)
	{
		int x = rand() % 10000;
		int y = rand() % 10000;
		int v = rand() % 2;
		if (v == 1)
		{
			x *= -1;
		}

		std::shared_ptr<USpriteRenderer> cloud = CreateDefaultSubObject<USpriteRenderer>();
		cloud->SetTexture("Cloud_1.png", true, 10.0f);
		cloud->SetupAttachment(RootComponent);
		cloud->SetRelativeLocation({ static_cast<float>(x), static_cast<float>(y), 9000.f - 100.f - i * 10.f});	// Temp
		cloud->SetOrder(1);
	}
}