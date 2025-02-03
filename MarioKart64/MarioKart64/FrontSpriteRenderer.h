#pragma once
#include <EngineCore/SpriteRenderer.h>

class FrontSpriteRenderer : public USpriteRenderer
{
public:
	FrontSpriteRenderer();
	~FrontSpriteRenderer();

	FrontSpriteRenderer(const FrontSpriteRenderer& _other) = delete;
	FrontSpriteRenderer(FrontSpriteRenderer&& _other) noexcept = delete;
	FrontSpriteRenderer& operator=(const FrontSpriteRenderer& _other) = delete;
	FrontSpriteRenderer& operator=(FrontSpriteRenderer&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Render(class UEngineCamera* _camera, float _deltaTime) override;
};
