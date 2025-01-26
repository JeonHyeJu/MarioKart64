#pragma once
#include <EngineCore/Widget.h>

class WImageCustomWidget : public UWidget
{
public:
	WImageCustomWidget();
	~WImageCustomWidget();

	WImageCustomWidget(const WImageCustomWidget& _other) = delete;
	WImageCustomWidget(WImageCustomWidget&& _other) noexcept = delete;
	WImageCustomWidget& operator=(const WImageCustomWidget& _other) = delete;
	WImageCustomWidget& operator=(WImageCustomWidget&& _other) noexcept = delete;

	void SetAutoScale(bool _Value);
	void SetAutoScaleRatio(float _Scale);
	void SetSprite(std::string_view _name, UINT _index = 0);
	FVector GetRealScaleOfSprite() const;
	void SetColor(const FVector& _color);
	void SetAutoColor(bool _val, uint8_t _startIdx=0);

protected:
	void Tick(float _deltaTime) override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	void ChangeAutoColor(float _deltaTime);

	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;
	class UEngineSprite* Sprite = nullptr;

	URenderUnit RenderUnit;
	FSpriteData SpriteData;
	FVector Color = FVector::ZERO;
	UINT CurIndex = 0;

	bool IsAutoColor = false;
	UINT AutoColorIdx = 0;
	UColor AutoColor = UColor::BLACK;
	int VAL = 0;

	std::vector<UColor> AutoColors = {
		{ 255, 0, 0, 255 },
		{ 255, 255, 0, 255 },
		{ 0, 255, 0, 255 },
		{ 0, 0, 255, 255 },
		{ 0, 255, 255, 255 },
		{ 255, 0, 255, 255},
	};
};
