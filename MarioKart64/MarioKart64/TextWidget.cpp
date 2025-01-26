#include "PreCompile.h"
#include "TextWidget.h"
//#include <EngineCore/ImageWidget.h>
#include "ImageCustomWidget.h"

const UINT ATextWidget::MAX_SIZE = 24;

ATextWidget::ATextWidget()
{
	Texts.reserve(MAX_SIZE);

	for (UINT i = 0; i < MAX_SIZE; ++i)
	{
		std::shared_ptr<WImageCustomWidget> ptr = CreateWidget<WImageCustomWidget>(0);
		ptr->SetSprite(SPRITE_NAME, 75);
		ptr->SetAutoScaleRatio(ScaleRatio);
		Texts.push_back(ptr.get());

		if (i == 0)
		{
			WideWidth = ptr->GetRealScaleOfSprite().X * .85f;
		}
	}

	Move(FVector::ZERO);
}

ATextWidget::~ATextWidget()
{

}

void ATextWidget::BeginPlay()
{
	AHUD::BeginPlay();
}

void ATextWidget::Tick(float _deltaTime)
{
	AHUD::Tick(_deltaTime);
}

void ATextWidget::SetScaleRatio(float _ratio)
{
	ScaleRatio = _ratio;
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetAutoScaleRatio(ScaleRatio);
		if (i == 0)
		{
			WideWidth = Texts[i]->GetRealScaleOfSprite().X * .85f;
		}
	}

	SetText(Text);	// Temp
}

void ATextWidget::SetText(std::string_view _text)
{
	Text = _text;

	float accMarginX = 0.f;
	for (size_t i = 0, size = Text.size(); i < size; ++i)
	{
		FVector margin = { WideWidth, 0.f };
		float subX = 0.f;
		int idx = Text[i] - '0';
		if (idx < 0)	// special characters
		{
			if (idx == -2)	// .
			{
				idx = 48;
				margin.X = WideWidth * .8f;
				margin.Y = -10.f;
				subX = -15.f;
			}
			else if (idx == -9)	// '
			{
				idx = 38;
				margin.Y = 10.f;
			}
			else if (idx == -13)	// # -> _cc
			{
				idx = 53;
				margin.Y = -10.f;
			}
			else if (idx == -14)	// "
			{
				idx = 39;
				margin.Y = 10.f;
			}
			else if (idx == -16)	// space
			{
				idx = 75;
			}
		}
		else if (idx >= A)	// char
		{
			idx -= 7;
		}
		// else: number

		Texts[i]->SetSprite(SPRITE_NAME, idx);
		Texts[i]->SetRelativeLocation(FVector{ InitLoc.X + accMarginX + subX, InitLoc.Y + margin.Y });

		accMarginX += margin.X;
	}
}

void ATextWidget::SetColor(const FVector& _color)
{
	Color = _color;
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetColor(_color);
	}
}

void ATextWidget::SetAutoColor(bool _val, uint8_t _startIdx)
{
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetAutoColor(_val, _startIdx);
	}
}

void ATextWidget::Move(const FVector& _vec)
{
	FVector prevVec = InitLoc;
	InitLoc = _vec;
	for (size_t j = 0, size1 = Texts.size(); j < size1; ++j)
	{
		FVector loc = Texts[j]->GetRelativeLocation();
		Texts[j]->SetRelativeLocation(FVector{ InitLoc.X + loc.X - prevVec.X, InitLoc.Y + loc.Y - prevVec.Y });
	}
}