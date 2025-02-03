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

	std::vector<ASelectButton*> temp1, temp2;
	temp1.reserve(4);
	temp2.reserve(3);

	Buttons.insert(std::make_pair(ELayer::RULE, temp1));
	Buttons.insert(std::make_pair(ELayer::CC, temp2));

	SelectedIdxs.insert(std::make_pair(ELayer::RULE, 0));
	SelectedIdxs.insert(std::make_pair(ELayer::CC, 0));
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
	RRect->SetAutoScaleRatio(.5f);
	RRect->SetRelativeLocation({ 0.f, 0.f, 0.f });
	
	RDownArrow->SetSprite("SelectGame", 16);
	RDownArrow->SetRelativeLocation({ 0.f, -135.f, 0.f });

	SetBgScale(RRect->GetRealScaleOfSprite());
	SetBgLocation(RRect->GetRelativeLocation() + FVector{ 0.f, 0.f, 1.f });
}

void AGameSelectBox::GetRectSize(float* _refW, float* _refH)
{
	FVector scale = RRect->GetRealScaleOfSprite();
	*_refW = scale.X;
	*_refH = scale.Y;
}

FVector AGameSelectBox::GetButtonSize(ELayer _layerIdx, int _idx)
{
	if (_idx < 0 || _idx >= Buttons[_layerIdx].size())
	{
		return FVector::ZERO;
	}

	return Buttons[_layerIdx][_idx]->GetRealScaleOfSprite();
}

void AGameSelectBox::AddObjectsToLayer(ELayer _idx, const std::vector<SButtonData>& _options)
{
	for (size_t i = 0, size = _options.size(); i < size; ++i)
	{
		const SButtonData& data = _options[i];

		std::shared_ptr<ASelectButton> ptr = GetWorld()->SpawnActor<ASelectButton>();
		ptr->Init(data.SpriteName, data.SpriteIndex, data.AutoScaleRatio);
		
		FVector scale = ptr->GetRealScaleOfSprite();
		FVector loc = FVector{ 0.f, i * -scale.Y, 0.f };
		ptr->SetActorLocation(data.InitLoc + loc);
		ptr->AttachToActor(this);
		ptr->SetActive(data.IsInitActive);

		Buttons[_idx].push_back(ptr.get());
	}
}

void AGameSelectBox::SetActiveToLayers(ELayer _idx, bool _val)
{
	for (size_t i = 0, size = Buttons[_idx].size(); i < size; ++i)
	{
		ASelectButton* btn = Buttons[_idx][i];
		btn->SetActive(_val);
	}
}

void AGameSelectBox::SetBlinkStateToLayer(ELayer _layerIdx, int _idx, EBlinkState _state)
{
	std::vector<ASelectButton*>& btns = Buttons[_layerIdx];
	size_t size = btns.size();
	for (size_t i = 0; i < size; ++i)
	{
		btns[i]->SetBlinkState(EBlinkState::OFF);
	}

	if (_idx >= 0 || _idx < size)
	{
		btns[_idx]->SetBlinkState(_state);
		SelectedIdxs[_layerIdx] = _idx;
	}
}

void AGameSelectBox::MoveSelectedLayer(ELayer _idx, EDirection _dir)
{
	if (_dir == EDirection::UP)
	{
		if (SelectedIdxs[_idx] > 0)
		{
			SetBlinkStateToLayer(_idx, --SelectedIdxs[_idx], EBlinkState::BLINK);
		}
	}
	else
	{
		if (SelectedIdxs[_idx] < Buttons[_idx].size() - 1)
		{
			SetBlinkStateToLayer(_idx, ++SelectedIdxs[_idx], EBlinkState::BLINK);
		}
	}
}

void AGameSelectBox::SetEnable(bool _val)
{
	for (std::pair<const ELayer, std::vector<ASelectButton*>>& data : Buttons)
	{
		std::vector<ASelectButton*>& vec = data.second;
		for (size_t i = 0, size = vec.size(); i < size; ++i)
		{
			vec[i]->SetEnable(_val);
		}
	}

	if (_val)
	{
		RRect->ColorData.MulColor = { 1.f, 1.f, 1.f, 1.f };
		RDownArrow->ColorData.MulColor = { 1.f, 1.f, 1.f, 1.f };
	}
	else
	{
		RRect->ColorData.MulColor = { .1f, .1f, .1f, 1.f };
		RDownArrow->ColorData.MulColor = { .1f, .1f, .1f, 1.f };
	}
}