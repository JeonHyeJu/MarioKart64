#include "PreCompile.h"
#include "TextWrapper.h"
#include "TextWidget.h"

WTextWrapper::WTextWrapper()
{
}

WTextWrapper::~WTextWrapper()
{
}

void WTextWrapper::Tick(float _deltaTime)
{
	WDefaultWidget::Tick(_deltaTime);
}

void WTextWrapper::Render(UEngineCamera* _camera, float _deltaTime)
{
	WDefaultWidget::Render(_camera, _deltaTime);
}

void WTextWrapper::InitUpperTexts(const std::vector<std::string> _texts)
{
	if (Texts.size() > 0) return;

	FVector initLoc{ 0.f, 300.f };

	for (size_t i = 0, size = _texts.size(); i < size; ++i)
	{
		WTextWidget* ptr = CreateWidget<WTextWidget>(0, HUD).get();
		Texts.push_back(ptr);

		if (i == 0)
		{
			ptr->SetScaleRatio(4.f);
			ptr->SetInitLoc(initLoc + FVector{ 100.f, 0.f });
			ptr->SetAutoColor(true);
		}
		else if (i == 1)
		{
			ptr->SetInitLoc(initLoc + FVector{ 175.f, TXT_MARGIN_H - 4.f });
			ptr->SetAutoColor(true, 5);
		}
		else
		{
			ptr->SetInitLoc(initLoc + FVector{ 0.f, TXT_MARGIN_H } *static_cast<float>(i));
			ptr->SetColor(Yellow);
		}

		ptr->SetText(_texts[i]);
	}

	Texts[2]->SetAutoColor(true, 0, 255);
}

void WTextWrapper::InitLowerTexts(const std::vector<std::string> _texts)
{
	if (Texts.size() > 0) return;

	FVector initLoc{ 0.f, -100.f };

	for (size_t i = 0, size = _texts.size(); i < size; ++i)
	{
		WTextWidget* ptr = CreateWidget<WTextWidget>(0, HUD).get();
		Texts.push_back(ptr);

		if (i == size - 1)
		{
			Texts.back()->SetScaleRatio(2.f);
			Texts.back()->SetInitLoc(initLoc + FVector{ 45.f, TXT_MARGIN_H * static_cast<float>(Texts.size() - 1) });
			Texts.back()->SetAutoColor(true, 3, 5);
		}
		else
		{
			Texts[i]->SetInitLoc(initLoc + FVector{ 0.f, TXT_MARGIN_H } *static_cast<float>(i));
			Texts[i]->SetColor(Yellow);
		}

		Texts[i]->SetText(_texts[i]);
	}
}