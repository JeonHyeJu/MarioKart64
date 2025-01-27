#include "PreCompile.h"
#include "TextWidget.h"
#include "ImageCustomWidget.h"

const UINT WTextWidget::MAX_SIZE = 24;

WTextWidget::WTextWidget()
{
	Texts.reserve(MAX_SIZE);

	for (UINT i = 0; i < MAX_SIZE; ++i)
	{
		std::shared_ptr<WImageCustomWidget> ptr = CreateWidget<WImageCustomWidget>(0, HUD);
		ptr->SetSprite(SPRITE_NAME, 75);
		ptr->SetAutoScaleRatio(ScaleRatio);
		Texts.push_back(ptr.get());

		if (i == 0)
		{
			WideWidth = ptr->GetRealScaleOfSprite().X * .85f;
		}
	}
}

WTextWidget::~WTextWidget()
{

}

void WTextWidget::Tick(float _deltaTime)
{
	WDefaultWidget::Tick(_deltaTime);
}

void WTextWidget::Render(UEngineCamera* _camera, float _deltaTime)
{
	WDefaultWidget::Render(_camera, _deltaTime);

	//CameraTransUpdate(_camera);
}

void WTextWidget::SetScaleRatio(float _ratio)
{
	ScaleRatio = _ratio;
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetAutoScaleRatio(ScaleRatio);
		if (i == 0)
		{
			if (ScaleRatio < 3.f)
			{
				WideWidth = Texts[i]->GetRealScaleOfSprite().X * .65f;
			}
			else
			{
				WideWidth = Texts[i]->GetRealScaleOfSprite().X * .85f;
			}
		}
	}

	SetText(Text);	// Temp
}

void WTextWidget::SetText(std::string_view _text)
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
		FVector move = FVector{ InitLoc.X + accMarginX + subX, InitLoc.Y + margin.Y };
		Texts[i]->SetRelativeLocation(move);

		accMarginX += margin.X;
	}
}

void WTextWidget::SetColor(const FVector& _color)
{
	Color = _color;
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetColor(_color);
	}
}

void WTextWidget::SetAutoColor(bool _val, uint8_t _startIdx, uint8_t _changeSpeed)
{
	for (size_t i = 0, size = Texts.size(); i < size; ++i)
	{
		Texts[i]->SetAutoColor(_val, _startIdx, _changeSpeed);
	}
}

void WTextWidget::SetInitLoc(const FVector& _vec)
{
	InitLoc = _vec;
}

void WTextWidget::SetMoveLoc(const FVector& _vec)
{
	MoveLoc = _vec;
}

void WTextWidget::Move(float _deltaTime)
{
	for (size_t i = 0, size1 = Texts.size(); i < size1; ++i)
	{
		FVector loc = Texts[i]->GetRelativeLocation();
		float sub = ((MoveLoc - loc) * _deltaTime * 1.f).X;
		if (abs(sub) < 1) continue;

		Texts[i]->AddRelativeLocation({ sub, 0.f, 0.f });
		//Texts[j]->SetRelativeLocation({ MoveLoc.X, 0.f, 0.f });
	}
}