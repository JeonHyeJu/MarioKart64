#include "PreCompile.h"
#include "ExpandEffect.h"
#include "CGlobal.h"

FxExpandEffect::FxExpandEffect()
{
}

FxExpandEffect::~FxExpandEffect()
{
}

void FxExpandEffect::Init()
{
	UPostEffect::Init();

	CopyTarget = std::make_shared<UEngineRenderTarget>();
	CopyTarget->CreateTarget(UEngineCore::GetScreenScale());

	RenderUnit.SetMesh("FullRect");
	RenderUnit.SetMaterial(CGlobal::EXPAND_EFFECT);
}

void FxExpandEffect::Effect(UEngineCamera* _camera, float _deltaTime)
{
	UPostEffect::Effect(_camera, _deltaTime);

	ResultTarget->CopyTo(CopyTarget);
	ResultTarget->Setting();

	RenderUnit.SetTexture("ImageTexture", CopyTarget->GetTexture());
	RenderUnit.Render(_camera, _deltaTime);
}
