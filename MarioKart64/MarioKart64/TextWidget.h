#pragma once
#include "DefaultWidget.h"

class WTextWidget : public WDefaultWidget
{
public:
	WTextWidget();
	~WTextWidget();

	WTextWidget(const WTextWidget& _other) = delete;
	WTextWidget(WTextWidget&& _other) noexcept = delete;
	WTextWidget& operator=(const WTextWidget& _other) = delete;
	WTextWidget& operator=(WTextWidget&& _other) noexcept = delete;

	void SetText(std::string_view _text);	// Have to do SetInitLoc() first..
	void Move(float _deltaTime);
	void SetInitLoc(const FVector& _vec);
	void SetMoveLoc(const FVector& _vec);
	void SetScaleRatio(float _ratio);
	void SetColor(const FVector& _color);
	void SetAutoColor(bool _val, uint8_t _startIdx=0, uint8_t _changeSpeed=15);

	static const UINT MAX_SIZE;

protected:
	void Tick(float _deltaTime) override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

	std::vector<class WImageCustomWidget*> Texts;

	const char* SPRITE_NAME = "FontAndPositions.png";
	std::string Text = "";
	FVector InitLoc = FVector::ZERO;
	FVector MoveLoc = FVector::ZERO;
	float ScaleRatio = 2.5f;
	float WideWidth = 0.f;
	int A = 17;

	FVector Color = FVector::ZERO;
};
