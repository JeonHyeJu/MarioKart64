#pragma once
#include "CData.h"
#include <EngineCore/Widget.h>

struct SLetterData
{
	float MinY = 0.f;
	float MaxY = 0.f;
};

class WLetterboxWidget : public UWidget
{
public:
	WLetterboxWidget();
	~WLetterboxWidget();

	WLetterboxWidget(const WLetterboxWidget& _other) = delete;
	WLetterboxWidget(WLetterboxWidget&& _other) noexcept = delete;
	WLetterboxWidget& operator=(const WLetterboxWidget& _other) = delete;
	WLetterboxWidget& operator=(WLetterboxWidget&& _other) noexcept = delete;

	void SetLocY(float _minY, float _maxY)
	{
		LetterData.MinY = _minY;
		LetterData.MaxY = _maxY;
	}

protected:
	void Tick(float _DeltaTime) override;
	void Render(class UEngineCamera* _camera, float _deltaTime) override;

private:
	SLetterData LetterData;
	URenderUnit RenderUnit;
};
