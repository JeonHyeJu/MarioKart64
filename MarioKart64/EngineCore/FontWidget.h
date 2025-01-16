#pragma once
#include "Widget.h"
#include "EngineFont.h"

class UFontWidget : public UWidget
{
public:
	ENGINEAPI UFontWidget();
	ENGINEAPI ~UFontWidget();

	UFontWidget(const UFontWidget& _Other) = delete;
	UFontWidget(UFontWidget&& _Other) noexcept = delete;
	UFontWidget& operator=(const UFontWidget& _Other) = delete;
	UFontWidget& operator=(UFontWidget&& _Other) noexcept = delete;

	void Tick(float _DeltaTime) override;

	void Render(UEngineCamera* Camera, float _DeltaTime) override;

	ENGINEAPI void SetFont(std::string_view _Value, float _Scale = 20.0f, UColor _Color = UColor::BLACK, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_LEFT);

	void SetText(std::string _Text)
	{
		Text = _Text;
	}

protected:

private:
	UEngineFont* Font = nullptr;
	std::string Text;
	float Scale = 20.0f;
	UColor Color = UColor::BLACK;
	FW1_TEXT_FLAG Flag = FW1_TEXT_FLAG::FW1_LEFT;
};

