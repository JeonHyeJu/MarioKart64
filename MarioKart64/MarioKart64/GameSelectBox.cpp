#include "PreCompile.h"
#include "GameSelectBox.h"
#include "SelectButton.h"
#include <EngineCore/SpriteRenderer.h>

AGameSelectBox::AGameSelectBox()
{
	ARuleList.reserve(4);
	ACCList.reserve(4);

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

	std::vector<EOptionType> vecR;
	std::vector<ECCType> vecC = { ECCType::C50, ECCType::C100, ECCType::C150 };
	vecR.reserve(4);

	if (_idx == 0)
	{
		vecR.emplace_back(EOptionType::MARIO_GP);
		vecR.emplace_back(EOptionType::TIME_TRAIALS);
	}
	else if (_idx == 1)
	{
		vecR.emplace_back(EOptionType::MARIO_GP);
		vecR.emplace_back(EOptionType::VS);
		vecR.emplace_back(EOptionType::BATTLE);
	}
	else if (_idx == 2)
	{
		vecR.emplace_back(EOptionType::VS);
		vecR.emplace_back(EOptionType::BATTLE);
	}
	else if (_idx == 3)
	{
		vecR.emplace_back(EOptionType::VS);
		vecR.emplace_back(EOptionType::BATTLE);
	}

	CreateRuleList(vecR);
	CreateCCList(vecC);
}

void AGameSelectBox::GetRectSize(float* _refW, float* _refH)
{
	FVector scale = RRect->GetRealScaleOfSprite();
	*_refW = scale.X;
	*_refH = scale.Y;
}

void AGameSelectBox::CreateRuleList(const std::vector<EOptionType>& _options)
{
	if (ARuleList.size() > 0)
	{
		MSGASSERT("You can't execute CreateOptionList() more than once.");
	}

	for (size_t i=0, size=_options.size(); i<size; ++i)
	{
		float SCALE = .6f;
		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		switch (_options[i])
		{
			case EOptionType::MARIO_GP:
				ptr->Init("SelectGame", 10, SCALE);
				break;
			case EOptionType::TIME_TRAIALS:
				ptr->Init("SelectGame", 9, SCALE);
				break;
			case EOptionType::VS:
				ptr->Init("SelectGame", 11, SCALE);
				break;
			case EOptionType::BATTLE:
				ptr->Init("SelectGame", 8, SCALE);
				break;
		}

		FVector scale = ptr->GetRealScaleOfSprite();
		ptr->SetActorLocation({ 0.f, -240.f + (i * -scale.Y), 0.f });
		ptr->AttachToActor(this);
		ARuleList.push_back(ptr.get());
	}
}

void AGameSelectBox::CreateCCList(const std::vector<ECCType>& _options)
{
	if (ACCList.size() > 0)
	{
		MSGASSERT("You can't execute CreateCCList() more than once.");
	}

	if (ARuleList.size() == 0)
	{
		MSGASSERT("You have to set ARuleList first.");
	}

	for (size_t i = 0, size = _options.size(); i < size; ++i)
	{
		float SCALE = .6f;
		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		switch (_options[i])
		{
		case ECCType::C50:
			ptr->Init("SelectGame", 4, SCALE);
			break;
		case ECCType::C100:
			ptr->Init("SelectGame", 5, SCALE);
			break;
		case ECCType::C150:
			ptr->Init("SelectGame", 6, SCALE);
			break;
		}

		float leftX = (*ARuleList.begin())->GetRealScaleOfSprite().X;
		FVector scale = ptr->GetRealScaleOfSprite();
		ptr->SetActorLocation({ leftX, -235.f + (i * -scale.Y), 0.f });
		ptr->AttachToActor(this);
		ptr->SetActive(false);
		ACCList.push_back(ptr.get());
	}
}

void AGameSelectBox::SetActiveCCList(bool _val)
{
	for (ASelectButton* _type : ACCList)
	{
		_type->SetActive(_val);
	}
}

void AGameSelectBox::SetBlinkStateToRuleList(int _idx, EBlinkState _state)
{
	size_t size = ARuleList.size();
	for (size_t i = 0; i < size; ++i)
	{
		ARuleList[i]->SetBlinkState(EBlinkState::OFF);
	}

	if (_idx >= 0 || _idx < size)
	{
		ARuleList[_idx]->SetBlinkState(_state);
		SelectedIdxRule = _idx;
	}
}

void AGameSelectBox::SetBlinkStateToCCList(int _idx, EBlinkState _state)
{
	size_t size = ACCList.size();
	for (size_t i = 0; i < size; ++i)
	{
		ACCList[i]->SetBlinkState(EBlinkState::OFF);
	}

	if (_idx >= 0 || _idx < size)
	{
		ACCList[_idx]->SetBlinkState(_state);
		SelectedIdxCC = _idx;
	}
}

void AGameSelectBox::UpBlinkStateToRuleList()
{
	if (SelectedIdxRule > 0)
	{
		SetBlinkStateToRuleList(--SelectedIdxRule, EBlinkState::BLINK);
	}
}

void AGameSelectBox::DownBlinkStateToRuleList()
{
	if (SelectedIdxRule < ARuleList.size() - 1)
	{
		SetBlinkStateToRuleList(++SelectedIdxRule, EBlinkState::BLINK);
	}
}

void AGameSelectBox::UpBlinkStateToCCList()
{
	if (SelectedIdxCC > 0)
	{
		SetBlinkStateToCCList(--SelectedIdxCC, EBlinkState::BLINK);
	}
}

void AGameSelectBox::DownBlinkStateToCCList()
{
	if (SelectedIdxCC < ACCList.size() - 1)
	{
		SetBlinkStateToCCList(++SelectedIdxCC, EBlinkState::BLINK);
	}
}
