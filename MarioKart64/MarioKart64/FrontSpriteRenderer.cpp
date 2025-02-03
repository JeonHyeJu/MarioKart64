#include "PreCompile.h"
#include "CGlobal.h"
#include "FrontSpriteRenderer.h"

FrontSpriteRenderer::FrontSpriteRenderer()
: USpriteRenderer(CGlobal::SPRITE_RENDERER)
{
}

FrontSpriteRenderer::~FrontSpriteRenderer()
{

}

void FrontSpriteRenderer::BeginPlay()
{
	USpriteRenderer::BeginPlay();
}

void FrontSpriteRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	USpriteRenderer::Render(_camera, _deltaTime);
}