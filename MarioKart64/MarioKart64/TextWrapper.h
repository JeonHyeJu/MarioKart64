#pragma once
#include "DefaultWidget.h"

class WTextWrapper : public WDefaultWidget
{
public:
	WTextWrapper();
	~WTextWrapper();

	WTextWrapper(const WTextWrapper& _other) = delete;
	WTextWrapper(WTextWrapper&& _other) noexcept = delete;
	WTextWrapper& operator=(const WTextWrapper& _other) = delete;
	WTextWrapper& operator=(WTextWrapper&& _other) noexcept = delete;

	void InitText(std::string_view _text, int _autoColorIdx=0);
	void InitUpperTexts(const std::vector<std::string>& _texts);
	void InitLowerTexts(const std::vector<std::string>& _texts);

protected:
	void Tick(float _deltaTime) override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	std::vector<class WTextWidget*> Texts;

	const float TXT_MARGIN_H = -50.f;
	FVector Yellow{ .8f, .8f, 0.f, 1.f };
};
