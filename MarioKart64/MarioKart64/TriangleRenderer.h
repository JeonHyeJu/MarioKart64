#pragma once
#include <EngineCore/Renderer.h>
#include "CData.h"

class TriangleRenderer : public URenderer
{
public:
	TriangleRenderer();
	~TriangleRenderer();

	TriangleRenderer(const TriangleRenderer& _other) = delete;
	TriangleRenderer(TriangleRenderer&& _other) noexcept = delete;
	TriangleRenderer& operator=(const TriangleRenderer& _other) = delete;
	TriangleRenderer& operator=(TriangleRenderer&& _other) noexcept = delete;

protected:
	void BeginPlay();
	void Render(class UEngineCamera* _camera, float _deltaTime);

private:
	FColor RenderColor;
};
