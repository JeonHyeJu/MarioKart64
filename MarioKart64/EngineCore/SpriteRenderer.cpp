#include "PreCompile.h"
#include "SpriteRenderer.h"

USpriteRenderer::USpriteRenderer()
{
}

USpriteRenderer::~USpriteRenderer()
{
}

void USpriteRenderer::SetSprite(std::string_view _Name, size_t _Index)
{
	SetTexture(_Name);
	SetSpriteData(_Index);
}

void USpriteRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

