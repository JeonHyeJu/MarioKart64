#include "PreCompile.h"
#include "SelectButton.h"
#include "ColorRenderer.h"
#include <EngineCore/SpriteRenderer.h>

ASelectButton::ASelectButton()
{
	// Already has RootComponent in ABlinkObject
	RBtn = CreateDefaultSubObject<USpriteRenderer>();
	RDisable = CreateDefaultSubObject<ColorRenderer>();

	RBtn->SetupAttachment(RootComponent);
	RDisable->SetupAttachment(RootComponent);
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

	FVector scale = RBtn->GetRealScaleOfSprite();
	RDisable->SetScale3D(scale);
	RDisable->SetColor({ 0.f, 0.f, 0.f, .5f });
	RDisable->SetRelativeLocation(RBtn->GetRelativeLocation() + FVector{ 0.f, 0.f, 0.f, -1.f });
	RDisable->SetActive(false);

	SetBgScale(scale);
	SetBgLocation(RBtn->GetRelativeLocation() + FVector{ 0.f, 0.f, 1.f });
}

void ASelectButton::SetImage(std::string_view _name, UINT idx)
{
	if (RBtn == nullptr) return;

	RBtn->SetSprite(_name.data(), idx);
}

void ASelectButton::SetAutoScaleRatio(float _val)
{
	if (RBtn == nullptr) return;

	RBtn->SetAutoScaleRatio(_val);
}

FVector ASelectButton::GetRealScaleOfSprite() const
{
	if (RBtn == nullptr)
	{
		return FVector::NONE;
	}

	return RBtn->GetRealScaleOfSprite();
}

// TODO: It's not working..
void ASelectButton::SetEnable(bool _val)
{
	if (_val)
	{
		RDisable->SetActive(false);
	}
	else
	{
		RDisable->SetActive(true);
	}
}