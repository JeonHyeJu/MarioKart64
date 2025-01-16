#include "PreCompile.h"
#include "BlinkActor.h"
#include "ColorRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>

ABlinkActor::ABlinkActor()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RBackground = CreateDefaultSubObject<ColorRenderer>();
	RBackground->SetColor(OFF_COLOR);
	RBackground->SetupAttachment(RootComponent);
}

ABlinkActor::~ABlinkActor()
{

}

void ABlinkActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ABlinkActor::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	if (IsSelected)
	{
		ChangeSelectedRectColor(_deltaTime);
	}
}

void ABlinkActor::SetBgScale(const FVector& _scale)
{
	RBackground->SetRelativeScale3D(_scale);
}

void ABlinkActor::SetBgLocation(const FVector& _loc)
{
	RBackground->SetRelativeLocation(_loc);
}

void ABlinkActor::ChangeSelectedRectColor(float _deltaTime)
{
	static float one = -1.f;

	if (RectBgVal <= 0)
	{
		RectBgVal = 0.f;
		one = 1.f;
	}
	else if (RectBgVal >= 1)
	{
		RectBgVal = 1.f;
		one = -1.f;
	}

	RectBgVal += one * _deltaTime;
	RBackground->SetColor({ RectBgVal, RectBgVal, RectBgVal, 1.f });
}

void ABlinkActor::SetSelected(bool _val)
{
	IsSelected = _val;

	if (!_val)
	{
		RectBgVal = 1.f;
		RBackground->SetColor(OFF_COLOR);
	}
}