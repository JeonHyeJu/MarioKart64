#pragma once
#include <EngineCore/Renderer.h>

enum ERenderType
{
	Normal,
	Instancing,
};


class InstanceRectRenderer : public URenderer
{
public:
	InstanceRectRenderer();
	~InstanceRectRenderer();

	InstanceRectRenderer(const InstanceRectRenderer& _other) = delete;
	InstanceRectRenderer(InstanceRectRenderer&& _other) noexcept = delete;
	InstanceRectRenderer& operator=(const InstanceRectRenderer& _other) = delete;
	InstanceRectRenderer& operator=(InstanceRectRenderer&& _other) noexcept = delete;

	void InstancingOn();

protected:
	void BeginPlay() override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;
	void RenderInstancing(class UEngineCamera* _Camera, float _deltaTime);

private:
	ERenderType RenderType = ERenderType::Normal;
};
