#pragma once
#include <EngineCore/Renderer.h>
#include "CData.h"

class CubeRenderer : public URenderer
{
public:
	CubeRenderer();
	~CubeRenderer();

	CubeRenderer(const CubeRenderer& _other) = delete;
	CubeRenderer(CubeRenderer&& _other) noexcept = delete;
	CubeRenderer& operator=(const CubeRenderer& _other) = delete;
	CubeRenderer& operator=(CubeRenderer&& _other) noexcept = delete;

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
