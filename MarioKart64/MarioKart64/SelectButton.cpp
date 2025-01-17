#include "PreCompile.h"
#include "SelectButton.h"
#include "ColorRenderer.h"
#include <EngineCore/SpriteRenderer.h>

ASelectButton::ASelectButton()
{
	// Already has RootComponent in ABlinkObject
	RBtn = CreateDefaultSubObject<USpriteRenderer>();
	RBtn->SetupAttachment(RootComponent);
}

ASelectButton::~ASelectButton()
{
}

void ASelectButton::BeginPlay()
{
	ABlinkActor::BeginPlay();
}

void ASelectButton::Tick(float _deltaTime)
{
	ABlinkActor::Tick(_deltaTime);
}

void ASelectButton::Init(std::string_view _name, UINT idx, float _scale)
{
	if (RBtn == nullptr) return;

	RBtn->SetSprite(_name.data(), idx);
	RBtn->SetAutoScaleRatio(_scale);

	SetBgScale(RBtn->GetRealScaleOfSprite());
	SetBgLocation(RBtn->GetRelativeLocation() + FVector{ 0.f, 0.f, 1.f });
}

void ASelectButton::SetImage(std::string_view _name, UINT idx)
{
	if (RBtn == nullptr) return;

	RBtn->SetSprite(_name.data(), idx);
}

FVector ASelectButton::GetRealScaleOfSprite() const
{
	if (RBtn == nullptr)
	{
		return FVector::NONE;
	}

	return RBtn->GetRealScaleOfSprite();
}