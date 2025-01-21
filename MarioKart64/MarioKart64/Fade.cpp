#include "PreCompile.h"
#include "Fade.h"
#include "CGlobal.h"
#include "ColorRenderer.h"

AFade::AFade()
{
	RRect = CreateDefaultSubObject<ColorRenderer>();
	RRect->SetScale3D({ CGlobal::FWINDOW_W, CGlobal::FWINDOW_H, 1.f });
	RRect->SetColor({ 0.f, 0.f, 0.f, 0.f });

	RootComponent = RRect;
}

AFade::~AFade()
{
}

void AFade::BeginPlay()
{
	AActor::BeginPlay();
}

void AFade::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	if (IsRun)
	{
		Update(_deltaTime);
	}
}

void AFade::Show()
{
	RRect->SetColor({ 0.f, 0.f, 0.f, 0.f });
	CurVal = 0.f;
	ChangeVal = 1.f;
	IsRun = true;
}

void AFade::Hide()
{
	RRect->SetColor({ 0.f, 0.f, 0.f, 1.f });
	CurVal = 1.f;
	ChangeVal = -1.f;
	IsRun = true;
}

void AFade::Update(float _deltaTime)
{
	// Show
	if (ChangeVal > 0)
	{
		if (CurVal < 1)
		{
			CurVal += ChangeVal * .1f * _deltaTime;
			if (CurVal > 1) CurVal = 1.f;
		}
		else
		{
			CurVal = 1.f;
			IsRun = false;
		}
	}
	// Hide
	else
	{
		if (CurVal > 0)
		{
			CurVal += ChangeVal * .1f * _deltaTime;
			if (CurVal < 0) CurVal = 0.f;
		}
		else
		{
			CurVal = 0.f;
			IsRun = false;
		}
	}

	RRect->SetColor(FVector{ 0.f, 0.f, 0.f, CurVal });
}