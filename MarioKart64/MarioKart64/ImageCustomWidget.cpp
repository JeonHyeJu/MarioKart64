#include "PreCompile.h"
#include "ImageCustomWidget.h"
#include "CGlobal.h"

WImageCustomWidget::WImageCustomWidget()
{
	RenderUnit.SetMesh("Rect");
	RenderUnit.SetMaterial(CGlobal::WIDGET_SHADER);

	SpriteData.CuttingPos = { 0.0f, 0.0f };
	SpriteData.CuttingSize = { 1.0f, 1.0f };
	SpriteData.Pivot = { 0.5f, 0.5f };

	RenderUnit.ConstantBufferLinkData("FSpriteData", SpriteData);
	RenderUnit.ConstantBufferLinkData("FText", Color);
}

WImageCustomWidget::~WImageCustomWidget()
{

}

void WImageCustomWidget::Tick(float _deltaTime)
{
	WDefaultWidget::Tick(_deltaTime);

	if (IsAutoColor)
	{
		ChangeAutoColor(_deltaTime);
	}
}

void WImageCustomWidget::Render(UEngineCamera* _camera, float _deltaTime)
{
	WDefaultWidget::Render(_camera, _deltaTime);

	if (true == IsAutoScale && nullptr != Sprite)
	{
		FVector Scale = Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}

	CameraTransUpdate(_camera);
	RenderUnit.Render(_camera, _deltaTime);
}

void WImageCustomWidget::SetAutoScale(bool _Value)
{
	IsAutoScale = _Value;
}

void WImageCustomWidget::SetAutoScaleRatio(float _Scale)
{
	AutoScaleRatio = _Scale;
}

void WImageCustomWidget::SetSprite(std::string_view _name, UINT _index)
{
	Sprite = UEngineSprite::Find<UEngineSprite>(_name).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("존재하지 않는 텍스처를 세팅하려고 했습니다.");
		return;
	}

	RenderUnit.SetTexture("diffTexture", Sprite->GetTexture(_index)->GetName());
	SpriteData = Sprite->GetSpriteData(_index);
	CurIndex = _index;
}

void WImageCustomWidget::SetColor(const FVector& _color)
{
	Color = _color;
}

void WImageCustomWidget::SetAutoColor(bool _val, uint8_t _startIdx, uint8_t _changeSpeed)
{
	if (_startIdx > 6)
	{
		MSGASSERT("자동 색상 범위를 넘어섰습니다.");
	}

	IsAutoColor = _val;
	AutoColorIdx = _startIdx;
	ChangeSpeed = _changeSpeed;

	Color.X = AutoColors[AutoColorIdx].R / 255.f;
	Color.Y = AutoColors[AutoColorIdx].G / 255.f;
	Color.Z = AutoColors[AutoColorIdx].B / 255.f;
	AutoColor = AutoColors[AutoColorIdx];
}

FVector WImageCustomWidget::GetRealScaleOfSprite() const
{
	if (CurIndex < 0 || Sprite == nullptr)
	{
		return FVector::NONE;
	}

	FVector scale = Sprite->GetSpriteScaleToReal(CurIndex);
	if (IsAutoScale)
	{
		return scale * AutoScaleRatio;
	}

	return scale;
}

void WImageCustomWidget::ChangeAutoColor(float _deltaTime)
{
	UINT nextIdx = AutoColorIdx + 1;
	if (AutoColorIdx >= AutoColors.size() - 1)
	{
		nextIdx = 0;
	}

	UColor nextColor = AutoColors[nextIdx];
	FVector addColor = FVector::NONE;
	if (AutoColor != nextColor)
	{
		UColor subColor = nextColor - AutoColor;
		if (subColor.R != 0)
		{
			float mulVal = 1.f;
			if (nextColor.R < AutoColor.R)
			{
				AutoColor.R -= ChangeSpeed;
				mulVal = -1.f;
			}
			else
			{
				AutoColor.R += ChangeSpeed;
			}
			addColor.X = static_cast<float>(ChangeSpeed * mulVal / 255.f);
		}
		if (subColor.G != 0)
		{
			float mulVal = 1.f;
			if (nextColor.G < AutoColor.G)
			{
				AutoColor.G -= ChangeSpeed;
				mulVal = -1.f;
			}
			else
			{
				AutoColor.G += ChangeSpeed;
			}
			addColor.Y = static_cast<float>(ChangeSpeed * mulVal / 255.f);
		}
		if (subColor.B != 0)
		{
			float mulVal = 1.f;
			if (nextColor.B < AutoColor.B)
			{
				AutoColor.B -= ChangeSpeed;
				mulVal = -1.f;
			}
			else
			{
				AutoColor.B += ChangeSpeed;
			}
			addColor.Z = static_cast<float>(ChangeSpeed * mulVal / 255.f);
		}

		Color += addColor;
	}
	else
	{
		if (++AutoColorIdx >= AutoColors.size())
		{
			AutoColorIdx = 0;
		}
	}
}