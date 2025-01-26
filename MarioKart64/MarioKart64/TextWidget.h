#pragma once
#include <EngineCore/HUD.h>

class ATextWidget : public AHUD
{
public:
	ATextWidget();
	~ATextWidget();

	ATextWidget(const ATextWidget& _other) = delete;
	ATextWidget(ATextWidget&& _other) noexcept = delete;
	ATextWidget& operator=(const ATextWidget& _other) = delete;
	ATextWidget& operator=(ATextWidget&& _other) noexcept = delete;

	void SetText(std::string_view _text);
	void Move(const FVector& _vec);		// have to set SetText first..
	void SetScaleRatio(float _ratio);
	void SetColor(const FVector& _color);
	void SetAutoColor(bool _val, uint8_t _startIdx=0);

	static const UINT MAX_SIZE;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	//std::vector<class UImageWidget*> Texts;
	std::vector<class WImageCustomWidget*> Texts;

	const char* SPRITE_NAME = "FontAndPositions.png";
	std::string Text = "";
	FVector InitLoc = FVector::ZERO;
	float ScaleRatio = 2.5f;
	float WideWidth = 0.f;
	int A = 17;

	FVector Color = FVector::ZERO;
};
