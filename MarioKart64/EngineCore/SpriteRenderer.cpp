#include "PreCompile.h"
#include "SpriteRenderer.h"

USpriteRenderer::USpriteRenderer()
{
}

USpriteRenderer::~USpriteRenderer()
{
}

void USpriteRenderer::SetSprite(std::string_view _Name)
{
	SetTexture(_Name);
}
