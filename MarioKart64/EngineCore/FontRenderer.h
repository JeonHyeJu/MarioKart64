#pragma once
#include "Renderer.h"
#include "EngineFont.h"

class UFontRenderer : public URenderer
{
public:
	ENGINEAPI UFontRenderer();
	ENGINEAPI ~UFontRenderer();

	UFontRenderer(const UFontRenderer& _Other) = delete;
	UFontRenderer(UFontRenderer&& _Other) noexcept = delete;
	UFontRenderer& operator=(const UFontRenderer& _Other) = delete;
	UFontRenderer& operator=(UFontRenderer&& _Other) noexcept = delete;

	ENGINEAPI void SetFont(std::string_view _Value, float _Scale = 20.0f, UColor _Color = UColor::BLACK, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_LEFT);

	void SetText(std::string _Text)
	{
		Text = _Text;
	}

	ENGINEAPI void Render(UEngineCamera* _Camera, float _DeltaTime) override;

protected:

private:
	UEngineFont* Font = nullptr;
	std::string Text;
	float Scale = 20.0f;
	UColor Color = UColor::BLACK;
	FW1_TEXT_FLAG Flag = FW1_TEXT_FLAG::FW1_LEFT;
};

