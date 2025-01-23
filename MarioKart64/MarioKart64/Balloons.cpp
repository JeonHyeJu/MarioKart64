#include "PreCompile.h"
#include "Balloons.h"
#include "CGlobal.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>

ABalloons::ABalloons()
{
	RootComponent = CreateDefaultSubObject<UDefaultSceneComponent>();

	Renderers.reserve(SIZE);
	srand(static_cast<UINT>(time(nullptr)));

	int winW = CGlobal::WINDOW_W;
	int winWH = CGlobal::WINDOW_W / 2;
	int winHH = CGlobal::WINDOW_H / 2;
	int winWHH = winWH / 2;
	int winHHH = winHH / 2;
	int Z = 500;

	const float ADD_COLOR = .25f;
	const float MARGIN_X = 10.f;
	for (int i = 0; i < SIZE; ++i)
	{
		float x = static_cast<float>(rand() % winW) - winWH;
		float y = static_cast<float>(rand() % winHH);
		float z = static_cast<float>(rand() % Z);
		int c1 = rand() % 2 + 1;
		int c2 = rand() % 2 + 1;
		int c3 = rand() % 2 + 1;
		FVector Color = FVector::NONE;

		if (c1 == 1)
		{
			Color.X = .5f;
		}
		if (c2 == 1)
		{
			Color.Y = .5f;
		}
		if (!(c1 == 1 && c2 == 1) && c3 == 1)
		{
			Color.Z = .5f;
		}
		if (c1 != 1 && c2 != 1 && c3 != 1)
		{
			Color.X = .5f;
		}

		std::shared_ptr<USpriteRenderer> ptr = CreateDefaultSubObject<USpriteRenderer>();
		ptr->SetSprite("Items.png", 24);
		ptr->SetAutoScaleRatio(1.f);
		ptr->SetRelativeLocation({ x, y, z-i });
		ptr->SetOrder(1);
		ptr->ColorData.PlusColor = Color;
		ptr->SetupAttachment(RootComponent);
		Renderers.push_back(ptr.get());
	}
}

ABalloons::~ABalloons()
{

}

void ABalloons::BeginPlay()
{
	AActor::BeginPlay();
}

void ABalloons::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	static float elapsedSec = 0.f;
	elapsedSec += _deltaTime;

	if (elapsedSec > .25f)
	{
		elapsedSec = 0.f;

		for (size_t i = 0, size = Renderers.size(); i < size; ++i)
		{
			int s = rand() % 3;
			float val = 0.f;

			if (s == 1)
			{
				val = 1.f;
			}
			else if (s == 2)
			{
				val = -1.f;
			}

			Renderers[i]->AddWorldRotation({ 0.f, 0.f, val * 50.f * _deltaTime });
		}
	}
}
