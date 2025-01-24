#include "PreCompile.h"
#include "ShrinkEffect.h"
#include "CGlobal.h"

FxShrinkEffect::FxShrinkEffect()
{
}

FxShrinkEffect::~FxShrinkEffect()
{
}

void FxShrinkEffect::Init()
{
	UPostEffect::Init();

	CopyTarget = std::make_shared<UEngineRenderTarget>();
	CopyTarget->CreateTarget(UEngineCore::GetScreenScale());

	RenderUnit.SetMesh("FullRect");
	RenderUnit.SetMaterial(CGlobal::SHRINK_EFFECT);
}

void FxShrinkEffect::Effect(UEngineCamera* _camera, float _deltaTime)
{
	UPostEffect::Effect(_camera, _deltaTime);

	ResultTarget->CopyTo(CopyTarget);
	ResultTarget->Setting();

	RenderUnit.SetTexture("ImageTexture", CopyTarget->GetTexture());
	RenderUnit.Render(_camera, _deltaTime);
}
