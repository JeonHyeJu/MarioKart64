#include "PreCompile.h"
#include "GameSelectBox.h"
#include "SelectButton.h"
#include <EngineCore/SpriteRenderer.h>

AGameSelectBox::AGameSelectBox()
{
	// Already has RootComponent in ABlinkObject
	RRect = CreateDefaultSubObject<USpriteRenderer>();
	RDownArrow = CreateDefaultSubObject<USpriteRenderer>();

	RRect->SetupAttachment(RootComponent);
	RDownArrow->SetupAttachment(RootComponent);
}

AGameSelectBox::~AGameSelectBox()
{

}

void AGameSelectBox::BeginPlay()
{
	ABlinkActor::BeginPlay();
}

void AGameSelectBox::Tick(float _deltaTime)
{
	ABlinkActor::Tick(_deltaTime);
}

void AGameSelectBox::Init(int _idx)
{
	RRect->SetSprite("SelectGame", _idx);
	RRect->SetAutoScaleRatio(.65f);
	RRect->SetRelativeLocation({ 0.f, 0.f, 0.f });
	
	RDownArrow->SetSprite("SelectGame", 16);
	RDownArrow->SetRelativeLocation({ 0.f, -175.f, 0.f });

	SetBgScale(RRect->GetRealScaleOfSprite());
	SetBgLocation(RRect->GetRelativeLocation() + FVector{ 0.f, 0.f, 1.f });

	std::vector<OptionType> vec;
	vec.reserve(4);

	if (_idx == 0)
	{
		vec.emplace_back(OptionType::MARIO_GP);
		vec.emplace_back(OptionType::TIME_TRAIALS);
	}
	else if (_idx == 1)
	{
		vec.emplace_back(OptionType::MARIO_GP);
		vec.emplace_back(OptionType::VS);
		vec.emplace_back(OptionType::BATTLE);
	}
	else if (_idx == 2)
	{
		vec.emplace_back(OptionType::VS);
		vec.emplace_back(OptionType::BATTLE);
	}
	else if (_idx == 3)
	{
		vec.emplace_back(OptionType::VS);
		vec.emplace_back(OptionType::BATTLE);
	}

	CreateOptionList(vec);
}

void AGameSelectBox::CreateOptionList(const std::vector<OptionType>& _options)
{
	if (RBtnOptions.size() > 0)
	{
		MSGASSERT("You can't execute CreateOptionList() more than one.");
	}

	for (size_t i=0, size=_options.size(); i<size; ++i)
	{
		float SCALE = .6f;
		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		switch (_options[i])
		{
			case OptionType::MARIO_GP:
				ptr->Init("SelectGame", 10, SCALE);
				break;
			case OptionType::TIME_TRAIALS:
				ptr->Init("SelectGame", 9, SCALE);
				break;
			case OptionType::VS:
				ptr->Init("SelectGame", 11, SCALE);
				break;
			case OptionType::BATTLE:
				ptr->Init("SelectGame", 8, SCALE);
				break;
		}

		FVector scale = ptr->GetRealScaleOfSprite();
		ptr->SetActorLocation({ 0.f, -240.f + (i * -scale.Y), 0.f });
		ptr->AttachToActor(this);
		RBtnOptions.push_back(ptr);
	}
}

void AGameSelectBox::GetRectSize(float* _refW, float* _refH)
{
	FVector scale = RRect->GetRealScaleOfSprite();
	*_refW = scale.X;
	*_refH = scale.Y;
}