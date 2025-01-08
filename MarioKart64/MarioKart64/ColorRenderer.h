#pragma once
#include <EngineCore/Renderer.h>
#include "CData.h"

class ColorRenderer : public URenderer
{
public:
	ColorRenderer();
	~ColorRenderer();

	ColorRenderer(const ColorRenderer& _Other) = delete;
	ColorRenderer(ColorRenderer&& _Other) noexcept = delete;
	ColorRenderer& operator=(const ColorRenderer& _Other) = delete;
	ColorRenderer& operator=(ColorRenderer&& _Other) noexcept = delete;

	void SetColor(const FVector& _color)
	{
		RenderColor.Albedo = _color;
	}

protected:
	void BeginPlay() override;
	void Render(class UEngineCamera* _camera, float _deltaTime) override;

private:
	FColor RenderColor;
};

