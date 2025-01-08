#pragma once
#include <EngineCore/Renderer.h>
#include "CData.h"

class LineRenderer : public URenderer
{
public:
	LineRenderer();
	~LineRenderer();

	LineRenderer(const LineRenderer& _other) = delete;
	LineRenderer(LineRenderer&& _other) noexcept = delete;
	LineRenderer& operator=(const LineRenderer& _other) = delete;
	LineRenderer& operator=(LineRenderer&& _other) noexcept = delete;

	void SetColor(const FVector& _color)
	{
		RenderColor.Albedo = _color;
	}

protected:
	void BeginPlay();
	void Render(class UEngineCamera* _camera, float _deltaTime);

private:
	FColor RenderColor;
};
