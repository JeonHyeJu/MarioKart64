#pragma once
#include "Renderer.h"

class USpriteRenderer : public URenderer
{
public:
	ENGINEAPI USpriteRenderer();
	ENGINEAPI ~USpriteRenderer();

	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	ENGINEAPI void SetSprite(std::string_view _Name, size_t _Index);

protected:
	void BeginPlay() override;

private:

};

